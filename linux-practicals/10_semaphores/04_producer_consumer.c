/*
 * File: 04_producer_consumer.c
 * Description: Classic Producer-Consumer Problem using POSIX Semaphores and Mutex.
 *              - 'empty' semaphore: Counts empty slots in circular buffer (initially BUFFER_SIZE).
 *              - 'full' semaphore : Counts filled slots in buffer (initially 0).
 *              - 'mutex'          : Enforces mutual exclusion during buffer access.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t sem_empty;
sem_t sem_full;
pthread_mutex_t buffer_mutex;

// Producer Thread Function
void *producer(void *arg) {
    for (int item = 1; item <= TOTAL_ITEMS; item++) {
        // Wait for an empty slot
        sem_wait(&sem_empty);

        // Enter critical section
        pthread_mutex_lock(&buffer_mutex);

        buffer[in] = item;
        printf("[Producer] Produced Item %2d -> Placed at Slot %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&buffer_mutex);

        // Signal that a full slot is available
        sem_post(&sem_full);

        usleep(100000); // 100ms delay
    }
    return NULL;
}

// Consumer Thread Function
void *consumer(void *arg) {
    for (int i = 1; i <= TOTAL_ITEMS; i++) {
        // Wait for a full slot
        sem_wait(&sem_full);

        // Enter critical section
        pthread_mutex_lock(&buffer_mutex);

        int item = buffer[out];
        printf("  [Consumer] Consumed Item %2d <- Retrieved from Slot %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&buffer_mutex);

        // Signal that an empty slot is now available
        sem_post(&sem_empty);

        usleep(150000); // 150ms delay
    }
    return NULL;
}

int main(void) {
    pthread_t prod_tid, cons_tid;

    printf("==========================================================\n");
    printf("         PRODUCER-CONSUMER PROBLEM (SEMAPHORES)           \n");
    printf("==========================================================\n");
    printf("Buffer Size: %d slots | Total items to exchange: %d\n\n", 
           BUFFER_SIZE, TOTAL_ITEMS);

    // Initialize semaphores and mutex
    sem_init(&sem_empty, 0, BUFFER_SIZE);
    sem_init(&sem_full, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);

    // Create threads
    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);

    // Wait for completion
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    // Clean up
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&buffer_mutex);

    printf("\n[Done] All %d items produced and consumed safely!\n", TOTAL_ITEMS);
    printf("==========================================================\n");

    return 0;
}
