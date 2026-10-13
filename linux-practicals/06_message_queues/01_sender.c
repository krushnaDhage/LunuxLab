/*
 * File: 01_sender.c (Process 1)
 * Description: Implements Process 1 of the private System V Message Queue conversation.
 *
 * Conversation Protocol:
 *  1. Process 1 sends (type 1): "Are you hearing me?"
 *  2. Process 1 waits for reply (type 2) from Process 2: "Loud and Clear"
 *  3. Process 1 sends (type 1): "I can hear you too"
 *  4. Process 1 cleans up the message queue using msgctl(IPC_RMID).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 128

// System V message buffer structure
struct msg_buffer {
    long mtype;
    char mtext[MSG_SIZE];
};

int main(void) {
    key_t key = ftok(".", 'M');
    if (key == -1) key = 12345; // Fallback constant key

    // Create or get message queue
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid < 0) {
        perror("msgget failed");
        return 1;
    }

    struct msg_buffer message;

    printf("==========================================================\n");
    printf("     PROCESS 1 (SENDER) - MESSAGE QUEUE (ID: %d)          \n", msgid);
    printf("==========================================================\n");

    // Step 1: Process 1 sends "Are you hearing me?"
    message.mtype = 1;
    strncpy(message.mtext, "Are you hearing me?", MSG_SIZE);
    printf("[Process 1] Sending message: \"%s\"\n", message.mtext);
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) < 0) {
        perror("msgsnd failed");
        return 1;
    }

    // Step 2: Process 1 waits for reply (type 2)
    printf("[Process 1] Waiting for reply from Process 2...\n");
    if (msgrcv(msgid, &message, sizeof(message.mtext), 2, 0) < 0) {
        perror("msgrcv failed");
        return 1;
    }
    printf("[Process 1] Received reply: \"%s\"\n", message.mtext);

    // Step 3: Process 1 sends "I can hear you too"
    message.mtype = 1;
    strncpy(message.mtext, "I can hear you too", MSG_SIZE);
    printf("[Process 1] Sending final message: \"%s\"\n", message.mtext);
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) < 0) {
        perror("msgsnd failed");
        return 1;
    }

    printf("\n[Process 1] Conversation sequence complete.\n");
    printf("[Process 1] Sleeping 2 seconds to allow Process 2 to receive final message...\n");
    sleep(2);

    // Step 4: Cleanup message queue
    msgctl(msgid, IPC_RMID, NULL);
    printf("[Process 1] Message queue removed from system.\n");

    return 0;
}
