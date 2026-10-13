/*
 * File: 01_parent_child_sum.c
 * Description: Two-way IPC between parent and child using two anonymous pipes:
 *              - Pipe 1 (Parent -> Child): Parent sends N integers to the child.
 *              - Pipe 2 (Child -> Parent): Child computes the sum and sends the result back.
 *              The parent reads and displays the final sum.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int p2c[2]; // Pipe 1: Parent to Child [0: read, 1: write]
    int c2p[2]; // Pipe 2: Child to Parent [0: read, 1: write]

    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
        perror("pipe creation failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // --- Child Process ---
        // Close unused pipe ends
        close(p2c[1]); // Child doesn't write to Pipe 1
        close(c2p[0]); // Child doesn't read from Pipe 2

        int n = 0;
        // Read count of integers N
        if (read(p2c[0], &n, sizeof(int)) <= 0) {
            perror("child read N failed");
            exit(1);
        }

        int *numbers = (int *)malloc(n * sizeof(int));
        // Read the N integers
        read(p2c[0], numbers, n * sizeof(int));
        close(p2c[0]); // Finished reading from parent

        long long sum = 0;
        for (int i = 0; i < n; i++) {
            sum += numbers[i];
        }
        free(numbers);

        printf("[Child PID %d] Received %d numbers. Calculated sum = %lld\n", 
               getpid(), n, sum);

        // Send sum back to parent
        write(c2p[1], &sum, sizeof(long long));
        close(c2p[1]); // Finished writing to parent

        exit(0);
    } else {
        // --- Parent Process ---
        // Close unused pipe ends
        close(p2c[0]); // Parent doesn't read from Pipe 1
        close(c2p[1]); // Parent doesn't write to Pipe 2

        int n;
        printf("==========================================================\n");
        printf("       PARENT-CHILD TWO-WAY PIPE COMMUNICATION            \n");
        printf("==========================================================\n");
        printf("Enter the count of integers (N): ");
        if (scanf("%d", &n) != 1 || n <= 0) {
            printf("Invalid count N.\n");
            return 1;
        }

        int *numbers = (int *)malloc(n * sizeof(int));
        printf("Enter %d integers separated by spaces:\n", n);
        for (int i = 0; i < n; i++) {
            if (scanf("%d", &numbers[i]) != 1) {
                printf("Invalid number entered.\n");
                free(numbers);
                return 1;
            }
        }

        // Send N followed by numbers to child
        write(p2c[1], &n, sizeof(int));
        write(p2c[1], numbers, n * sizeof(int));
        close(p2c[1]); // Close to signal write completion
        free(numbers);

        // Wait to read sum from child
        long long sum = 0;
        read(c2p[0], &sum, sizeof(long long));
        close(c2p[0]);

        // Wait for child to exit
        wait(NULL);

        printf("----------------------------------------------------------\n");
        printf("[Parent PID %d] Final Sum received from Child: %lld\n", getpid(), sum);
        printf("==========================================================\n");
    }

    return 0;
}
