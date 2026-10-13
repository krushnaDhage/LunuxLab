/*
 * File: 03_zombie_process.c
 * Description: Demonstrates creation of a Zombie process.
 *              The child process terminates immediately while the parent
 *              sleeps without calling wait(). During this period, the child
 *              remains in the process table as an entry with state 'Z' (<defunct>).
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child terminates immediately
        printf("[Child] PID: %d, terminating now...\n", getpid());
        exit(0);
    } else {
        // Parent sleeps without calling wait()
        printf("[Parent] PID: %d, Child PID: %d\n", getpid(), pid);
        printf("[Parent] Child has terminated but Parent is sleeping for 12 seconds.\n");
        printf("[Parent] Run 'ps -l' or 'ps aux | grep defunct' in another terminal.\n");
        printf("[Parent] Observe status 'Z' (Zombie) for Child PID %d.\n", pid);

        sleep(12);

        // Clean up zombie to be a well-behaved program
        wait(NULL);
        printf("[Parent] wait() called. Zombie reaped. Exiting.\n");
    }

    return 0;
}
