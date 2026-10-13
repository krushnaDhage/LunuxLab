/*
 * File: 03_semaphore_v_operation.c
 * Description: Performs the V (Signal / Increment) operation on the System V semaphore.
 *              Increments the semaphore value by 1, potentially waking up a process
 *              blocked in a P operation.
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
    sb.sem_op = 1;    // V operation: increment by 1 (Signal)
    sb.sem_flg = 0;

    printf("==========================================================\n");
    printf("         SEMAPHORE V (SIGNAL) OPERATION                   \n");
    printf("==========================================================\n");
    printf("Attempting V operation on Semaphore ID: %d...\n", semid);

    if (semop(semid, &sb, 1) < 0) {
        perror("semop (V operation) failed");
        return 1;
    }

    printf("\n[SUCCESS] V operation (Signal / Post) completed successfully!\n");
    printf("Semaphore incremented by PID: %d.\n", getpid());
    printf("==========================================================\n");

    return 0;
}
