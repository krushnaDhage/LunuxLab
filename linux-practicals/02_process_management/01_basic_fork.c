/*
 * File: 01_basic_fork.c
 * Description: Demonstrates basic fork() system call.
 *              The parent and child processes print messages.
 *              Notice that the order of execution between parent and child
 *              is non-deterministic and depends on the OS scheduler.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid;

    printf("Before fork(): Starting single process (PID: %d)\n", getpid());

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("[Child Process] Hello from Child! (My PID: %d, Parent PID: %d)\n", 
               getpid(), getppid());
    } else {
        // Parent process
        printf("[Parent Process] Hello from Parent! (My PID: %d, Created Child PID: %d)\n", 
               getpid(), pid);
    }

    printf("Common exit: Process with PID %d terminating.\n", getpid());
    return 0;
}
