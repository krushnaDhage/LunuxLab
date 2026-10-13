/*
 * File: 01_shared_memory_string.c
 * Description: Demonstrates System V Shared Memory IPC passing a string
 *              between two processes running simultaneously.
 *
 * Flow:
 *  1. Creates shared memory segment using shmget().
 *  2. Attaches the segment using shmat().
 *  3. Parent writes a string into shared memory.
 *  4. Child reads the string from the shared memory and prints it.
 *  5. Detaches with shmdt() and removes segment with shmctl(IPC_RMID).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main(void) {
    // 1. Create shared memory segment (private to this process and children)
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return 1;
    }

    printf("==========================================================\n");
    printf("         SHARED MEMORY STRING SHARING DEMO                \n");
    printf("==========================================================\n");
    printf("Created Shared Memory Segment (shmid: %d, size: %d bytes)\n", shmid, SHM_SIZE);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    } else if (pid == 0) {
        // --- Child Process (Reader) ---
        // Sleep briefly to ensure parent has written
        sleep(1);

        // Attach shared memory
        char *shared_str = (char *)shmat(shmid, NULL, 0);
        if (shared_str == (char *)-1) {
            perror("child shmat failed");
            exit(1);
        }

        printf("[Child PID %d] Attached to shared memory.\n", getpid());
        printf("[Child PID %d] Read string from shared memory:\n", getpid());
        printf("              \"%s\"\n", shared_str);

        // Detach from shared memory
        shmdt(shared_str);
        exit(0);
    } else {
        // --- Parent Process (Writer) ---
        // Attach shared memory
        char *shared_str = (char *)shmat(shmid, NULL, 0);
        if (shared_str == (char *)-1) {
            perror("parent shmat failed");
            return 1;
        }

        const char *msg = "Greetings from Parent Process via System V Shared Memory!";
        strncpy(shared_str, msg, SHM_SIZE);
        printf("[Parent PID %d] Wrote message into shared memory:\n", getpid());
        printf("               \"%s\"\n", msg);

        // Wait for child to read
        wait(NULL);

        // Detach from shared memory
        shmdt(shared_str);

        // Remove shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
        printf("[Parent] Cleaned up and removed shared memory segment %d.\n", shmid);
        printf("==========================================================\n");
    }

    return 0;
}
