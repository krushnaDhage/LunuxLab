/*
 * File: 01_odd_even_prime_threads.c
 * Description: Creates three threads using POSIX Threads (pthreads):
 *              - Thread 1: Identifies Odd numbers up to N and computes their average.
 *              - Thread 2: Identifies Even numbers up to N and computes their average.
 *              - Thread 3: Identifies Prime numbers up to N and computes their average.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define LIMIT 50

// Helper to check if a number is prime
bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Thread 1: Odd numbers
void *odd_thread(void *arg) {
    long long sum = 0;
    int count = 0;

    printf("[Odd Thread] Odd numbers up to %d:\n  ", LIMIT);
    for (int i = 1; i <= LIMIT; i++) {
        if (i % 2 != 0) {
            printf("%d ", i);
            sum += i;
            count++;
        }
    }
    double avg = (count > 0) ? (double)sum / count : 0.0;
    printf("\n[Odd Thread] Count: %d, Sum: %lld, Average: %.2f\n\n", count, sum, avg);
    pthread_exit(NULL);
}

// Thread 2: Even numbers
void *even_thread(void *arg) {
    long long sum = 0;
    int count = 0;

    printf("[Even Thread] Even numbers up to %d:\n  ", LIMIT);
    for (int i = 1; i <= LIMIT; i++) {
        if (i % 2 == 0) {
            printf("%d ", i);
            sum += i;
            count++;
        }
    }
    double avg = (count > 0) ? (double)sum / count : 0.0;
    printf("\n[Even Thread] Count: %d, Sum: %lld, Average: %.2f\n\n", count, sum, avg);
    pthread_exit(NULL);
}

// Thread 3: Prime numbers
void *prime_thread(void *arg) {
    long long sum = 0;
    int count = 0;

    printf("[Prime Thread] Prime numbers up to %d:\n  ", LIMIT);
    for (int i = 1; i <= LIMIT; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
            sum += i;
            count++;
        }
    }
    double avg = (count > 0) ? (double)sum / count : 0.0;
    printf("\n[Prime Thread] Count: %d, Sum: %lld, Average: %.2f\n\n", count, sum, avg);
    pthread_exit(NULL);
}

int main(void) {
    pthread_t t_odd, t_even, t_prime;

    printf("=== Multithreading: Odd, Even, Prime Averages (Limit: %d) ===\n\n", LIMIT);

    // Create the three threads
    pthread_create(&t_odd, NULL, odd_thread, NULL);
    pthread_create(&t_even, NULL, even_thread, NULL);
    pthread_create(&t_prime, NULL, prime_thread, NULL);

    // Wait for all threads to finish
    pthread_join(t_odd, NULL);
    pthread_join(t_even, NULL);
    pthread_join(t_prime, NULL);

    printf("=== All three worker threads completed ===\n");
    return 0;
}
