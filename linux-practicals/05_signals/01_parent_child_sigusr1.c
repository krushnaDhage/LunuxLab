/*
 * File: 01_parent_child_sigusr1.c
 * Description: Parent-child signal communication using SIGUSR1.
 *              The child process registers a signal handler for SIGUSR1,
 *              prints its PID, and waits for a signal using pause().
 *              The parent process sleeps for 3 seconds and then sends
 *              SIGUSR1 to the child using kill().
 *              The child handles the signal and displays: "Parent has sent SIGUSR1".
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Signal handler function in child
void sigusr1_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("[Child Handler] Parent has sent SIGUSR1\n");
    }
}

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // --- Child Process ---
        // Register signal handler for SIGUSR1
        signal(SIGUSR1, sigusr1_handler);

        printf("[Child PID %d] Signal handler installed. Waiting for SIGUSR1...\n", getpid());

        // Wait until any signal is received
        pause();

        printf("[Child PID %d] Resumed after signal. Terminating normally.\n", getpid());
        exit(0);
    } else {
        // --- Parent Process ---
        printf("[Parent PID %d] Created child PID %d. Waiting for 3 seconds...\n", getpid(), pid);
        sleep(3);

        printf("[Parent] Sending SIGUSR1 to child process %d via kill()...\n", pid);
        if (kill(pid, SIGUSR1) < 0) {
            perror("kill failed");
            return 1;
        }

        // Wait for child to exit cleanly
        wait(NULL);
        printf("[Parent] Child has finished. Parent exiting.\n");
    }

    return 0;
}
