/*
 * File: 02_receiver.c (Process 2)
 * Description: Implements Process 2 of the private System V Message Queue conversation.
 *
 * Conversation Protocol:
 *  1. Process 2 receives (type 1) from Process 1: "Are you hearing me?"
 *  2. Process 2 replies (type 2): "Loud and Clear"
 *  3. Process 2 receives (type 1) from Process 1: "I can hear you too"
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
    if (key == -1) key = 12345; // Same fallback key

    // Connect to existing message queue
    int msgid = msgget(key, 0666);
    if (msgid < 0) {
        printf("[Process 2] Message queue not found. Start 01_sender first!\n");
        perror("msgget failed");
        return 1;
    }

    struct msg_buffer message;

    printf("==========================================================\n");
    printf("     PROCESS 2 (RECEIVER) - MESSAGE QUEUE (ID: %d)        \n", msgid);
    printf("==========================================================\n");

    // Step 1: Wait to receive initial message (type 1)
    printf("[Process 2] Waiting for message from Process 1...\n");
    if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) < 0) {
        perror("msgrcv failed");
        return 1;
    }
    printf("[Process 2] Received message: \"%s\"\n", message.mtext);

    // Step 2: Send reply (type 2)
    message.mtype = 2;
    strncpy(message.mtext, "Loud and Clear", MSG_SIZE);
    printf("[Process 2] Replying with: \"%s\"\n", message.mtext);
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) < 0) {
        perror("msgsnd failed");
        return 1;
    }

    // Step 3: Wait for confirmation (type 1)
    printf("[Process 2] Waiting for final confirmation from Process 1...\n");
    if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) < 0) {
        perror("msgrcv failed");
        return 1;
    }
    printf("[Process 2] Received final message: \"%s\"\n", message.mtext);

    printf("\n[Process 2] Conversation completed successfully.\n");
    return 0;
}
