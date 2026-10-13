/*
 * File: 02_shared_loop_variable.c
 * Description: Demonstrates the classic data race caused by passing the address
 *              of a changing loop variable (&i) to pthread_create().
 *
 * Classroom Observation:
 *  All threads receive a pointer pointing to the SAME memory location on the
 *  stack of main(). As the loop increments 'i' rapidly, threads dereference *p
 *  after 'i' has already advanced, often printing identical numbers (like 5, 5, 5)
 *  instead of the expected 0, 1, 2, 3, 4.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_function(void *arg) {
    int *p = (int *)arg;
    // Dereferencing the shared address *p
    printf("Thread ID: %lu, Value of i received = %d\n", 
           (unsigned long)pthread_self(), *p);
    return NULL;
}

int main(void) {
    pthread_t tid[5];
    int i;

    printf("=== Passing Address of Loop Variable (&i) ===\n");
    printf("Expected: 0, 1, 2, 3, 4 | Observed: Often duplicate/unexpected values!\n\n");

    for (i = 0; i < 5; i++) {
        // PASSING ADDRESS &i (Vulnerable to data race!)
        pthread_create(&tid[i], NULL, thread_function, &i);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("\nNote: To fix this race condition, pass value directly as:\n");
    printf("      pthread_create(&tid[i], NULL, thread_function, (void *)(long)i);\n");

    return 0;
}
