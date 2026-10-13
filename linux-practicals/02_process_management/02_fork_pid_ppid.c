/*
 * File: 02_fork_pid_ppid.c
 * Description: Demonstrates PID and PPID relationships using sleep(5).
 *              Both parent and child sleep for 5 seconds to allow verifying
 *              in another terminal using 'ps -ef | grep fork' that child's
 *              PPID matches parent's PID.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child Process
        printf("[Child] PID: %d, PPID: %d\n", getpid(), getppid());
        printf("[Child] Sleeping for 5 seconds... Check 'ps -f' in another terminal.\n");
        sleep(5);
        printf("[Child] Woke up and exiting.\n");
    } else {
        // Parent Process
        printf("[Parent] PID: %d, Child PID: %d\n", getpid(), pid);
        printf("[Parent] Sleeping for 5 seconds... Check 'ps -f' in another terminal.\n");
        sleep(5);
        printf("[Parent] Woke up. Waiting for child to terminate...\n");
        wait(NULL);
        printf("[Parent] Child finished. Parent exiting.\n");
    }

    return 0;
}
