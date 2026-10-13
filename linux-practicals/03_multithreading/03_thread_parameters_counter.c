/*
 * File: 03_thread_parameters_counter.c
 * Description: Demonstrates passing distinct parameters (1 and 2) to two threads
 *              and incrementing a shared global counter WITHOUT synchronization.
 *
 * Inconsistency Observed:
 *  Because there is no mutex protecting the shared counter, race conditions
 *  can occur where increments are interleaved unexpectedly, resulting in
 *  duplicate counter prints or non-sequential values.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Global shared counter
int shared_counter = 0;

void *worker_thread(void *arg) {
    long param = (long)arg;

    printf("[Start] Thread ID: %lu received parameter: %ld\n", 
           (unsigned long)pthread_self(), param);

    // Print parameter 10 times and increment shared counter
    for (int i = 1; i <= 10; i++) {
        // Read-modify-write on shared counter without lock
        int temp = shared_counter;
        usleep(1000); // Small delay to highlight race conditions
        shared_counter = temp + 1;

        printf("Thread %ld (TID %lu) -> Param: %ld | Iteration: %2d | Shared Counter: %2d\n",
               param, (unsigned long)pthread_self(), param, i, shared_counter);
    }

    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    printf("=== Unsynchronized Shared Counter with Two Threads ===\n");
    printf("Initial counter value: %d\n\n", shared_counter);

    // Create threads passing parameters 1 and 2 by value
    pthread_create(&t1, NULL, worker_thread, (void *)1L);
    pthread_create(&t2, NULL, worker_thread, (void *)2L);

    // Wait for both threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nFinal Counter Value: %d (Expected 20 if perfectly serialized)\n", shared_counter);
    return 0;
}
