/*
 * File: 04_mutex_counter.c
 * Description: Demonstrates synchronization using pthread_mutex_t.
 *              Protects the shared counter critical section with a mutex.
 *
 * Key Viva/Theory Points:
 *  1. Mutual Exclusion: A mutex guarantees that only one thread executes
 *     the critical section at any instant, preventing data corruption.
 *     The final counter is GUARANTEED to be 20.
 *  2. Scheduling Order: A mutex ALONE DOES NOT guarantee alternating execution
 *     or a specific thread order. Thread 1 might acquire the mutex multiple
 *     times before Thread 2 gets CPU time.
 *  3. To enforce strict alternation, Condition Variables or Semaphores are required.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int shared_counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *worker_thread(void *arg) {
    long param = (long)arg;

    for (int i = 1; i <= 10; i++) {
        // --- Critical Section Start ---
        pthread_mutex_lock(&counter_mutex);

        shared_counter++;
        printf("Thread %ld (TID %lu) -> Param: %ld | Iteration: %2d | Shared Counter: %2d\n",
               param, (unsigned long)pthread_self(), param, i, shared_counter);

        pthread_mutex_unlock(&counter_mutex);
        // --- Critical Section End ---

        usleep(500); // Allow other thread opportunity to schedule
    }

    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    printf("=== Mutex-Protected Shared Counter ===\n");
    printf("Initial counter value: %d\n\n", shared_counter);

    pthread_create(&t1, NULL, worker_thread, (void *)1L);
    pthread_create(&t2, NULL, worker_thread, (void *)2L);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroy mutex
    pthread_mutex_destroy(&counter_mutex);

    printf("\nFinal Counter Value: %d (Accurately 20!)\n", shared_counter);
    return 0;
}
