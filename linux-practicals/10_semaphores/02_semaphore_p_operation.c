/*
 * File: 02_semaphore_p_operation.c
 * Description: Performs the P (Wait / Decrement) operation on the System V semaphore.
 *              Decrements the semaphore value by 1. If value is 0, it blocks until
 *              a V operation is executed by another process.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#define SEM_KEY 56789

int main(void) {
    // Connect to existing semaphore
    int semid = semget(SEM_KEY, 1, 0666);
    if (semid < 0) {
        printf("[Error] Semaphore not found. Run './01_semaphore_initialize' first!\n");
        perror("semget failed");
        return 1;
    }

    struct sembuf sb;
    sb.sem_num = 0;   // Semaphore index 0
    sb.sem_op = -1;   // P operation: decrement by 1 (Wait)
    sb.sem_flg = 0;   // Blocking call

    printf("==========================================================\n");
    printf("         SEMAPHORE P (WAIT) OPERATION                     \n");
    printf("==========================================================\n");
    printf("Attempting P operation on Semaphore ID: %d...\n", semid);
    printf("(Will block if semaphore value is 0 until V operation runs)\n");

    if (semop(semid, &sb, 1) < 0) {
        perror("semop (P operation) failed");
        return 1;
    }

    printf("\n[SUCCESS] P operation (Wait / Lock) completed successfully!\n");
    printf("Critical section acquired by PID: %d.\n", getpid());
    printf("==========================================================\n");

    return 0;
}
