/*
 * File: 05_alternating_thread_file_write.c
 * Description: Two threads write counter values to a file in strictly alternating order.
 *              Thread 1 writes odd turns, Thread 2 writes even turns.
 *              Uses POSIX pthread_mutex_t and pthread_cond_t (wait/signal mechanism)
 *              to enforce strict interleaving.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TOTAL_WRITES 10 // Each thread writes 5 times alternately

FILE *fp = NULL;
int counter = 1;
int turn = 1; // 1 for Thread 1, 2 for Thread 2

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_turn = PTHREAD_COND_INITIALIZER;

// Function for Thread 1 (writes when turn == 1)
void *thread_one(void *arg) {
    for (int i = 0; i < TOTAL_WRITES / 2; i++) {
        pthread_mutex_lock(&file_mutex);

        // Wait until it is Thread 1's turn
        while (turn != 1) {
            pthread_cond_wait(&cond_turn, &file_mutex);
        }

        // Write to file and print to console
        fprintf(fp, "[Thread 1] Counter Value: %d\n", counter);
        fflush(fp);
        printf("[Console] Thread 1 wrote counter %d to file.\n", counter);
        counter++;

        // Pass turn to Thread 2 and signal it
        turn = 2;
        pthread_cond_signal(&cond_turn);

        pthread_mutex_unlock(&file_mutex);
    }
    return NULL;
}

// Function for Thread 2 (writes when turn == 2)
void *thread_two(void *arg) {
    for (int i = 0; i < TOTAL_WRITES / 2; i++) {
        pthread_mutex_lock(&file_mutex);

        // Wait until it is Thread 2's turn
        while (turn != 2) {
            pthread_cond_wait(&cond_turn, &file_mutex);
        }

        // Write to file and print to console
        fprintf(fp, "[Thread 2] Counter Value: %d\n", counter);
        fflush(fp);
        printf("[Console] Thread 2 wrote counter %d to file.\n", counter);
        counter++;

        // Pass turn to Thread 1 and signal it
        turn = 1;
        pthread_cond_signal(&cond_turn);

        pthread_mutex_unlock(&file_mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    const char *filename = "alternating_output.txt";

    fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open output file");
        return 1;
    }

    printf("=== Strictly Alternating File Writes Between Two Threads ===\n");
    printf("Writing alternating counter values to '%s'...\n\n", filename);

    pthread_create(&t1, NULL, thread_one, NULL);
    pthread_create(&t2, NULL, thread_two, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    fclose(fp);
    pthread_mutex_destroy(&file_mutex);
    pthread_cond_destroy(&cond_turn);

    printf("\nAlternating writes complete. Check contents with: cat %s\n", filename);
    return 0;
}
