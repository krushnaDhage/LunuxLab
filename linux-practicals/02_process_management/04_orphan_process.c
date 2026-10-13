/*
 * File: 04_orphan_process.c
 * Description: Demonstrates creation of an Orphan process.
 *              The parent process exits immediately while the child process
 *              continues to sleep. The orphaned child is adopted by init
 *              (PID 1) or systemd / user systemd manager.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child Process
        printf("[Child] PID: %d, Original Parent PPID: %d\n", getpid(), getppid());
        printf("[Child] Sleeping for 4 seconds while parent terminates...\n");
        sleep(4);
        
        // After parent exits, child's PPID changes to init/systemd
        printf("[Child] Woke up! New Adopted Parent PPID: %d\n", getppid());
        printf("[Child] Child execution finished.\n");
    } else {
        // Parent Process exits immediately
        printf("[Parent] PID: %d created Child PID: %d. Parent exiting immediately!\n", 
               getpid(), pid);
        exit(0);
    }

    return 0;
}
