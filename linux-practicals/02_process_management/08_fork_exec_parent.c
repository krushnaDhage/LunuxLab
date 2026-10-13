/*
 * File: 08_fork_exec_parent.c
 * Description: Demonstrates calling execl() in the PARENT process after fork().
 *              When the parent executes execl(), its code is replaced by the
 *              new executable (/bin/date). The child continues its own code,
 *              becoming orphaned once the parent finishes.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    printf("[Main] Initial PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process continues executing normally
        printf("[Child PID %d] Running normally. Parent PID is %d.\n", getpid(), getppid());
        printf("[Child] Sleeping for 2 seconds...\n");
        sleep(2);
        printf("[Child] After sleep, my parent PPID is %d. Child terminating.\n", getppid());
    } else {
        // Parent process replaces itself via execl
        printf("[Parent PID %d] Replacing parent image with '/bin/date'...\n", getpid());
        execl("/bin/date", "date", NULL);

        // If execl fails:
        perror("execl failed in parent");
    }

    return 0;
}
