/*
 * File: 01_semaphore_initialize.c
 * Description: Initializes a System V Semaphore and displays its ID.
 *              Sets the semaphore initial value (e.g., 1 for mutual exclusion).
 * Usage: ./01_semaphore_initialize [initial_value]
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#define SEM_KEY 56789

// Required semun union for semctl
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(int argc, char *argv[]) {
    int init_val = (argc > 1) ? atoi(argv[1]) : 1;
    if (init_val < 0) init_val = 1;

    // Create a semaphore set with 1 semaphore
    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget failed");
        return 1;
    }

    union semun arg;
    arg.val = init_val;

    // Set initial value using semctl SETVAL
    if (semctl(semid, 0, SETVAL, arg) < 0) {
        perror("semctl SETVAL failed");
        return 1;
    }

    printf("==========================================================\n");
    printf("         SEMAPHORE INITIALIZATION                         \n");
    printf("==========================================================\n");
    printf(" Semaphore Key  : %d\n", SEM_KEY);
    printf(" Semaphore ID   : %d\n", semid);
    printf(" Initial Value  : %d\n", init_val);
    printf(" Status         : Initialized and ready for P and V operations.\n");
    printf("==========================================================\n");

    return 0;
}
