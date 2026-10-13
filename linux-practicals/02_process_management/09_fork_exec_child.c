/*
 * File: 09_fork_exec_child.c
 * Description: Demonstrates calling execl() in the CHILD process after fork().
 *              This is the classic Unix shell pattern:
 *              Parent forks a child. Child replaces its memory image with
 *              the desired executable (/bin/echo). The parent waits for the
 *              child using wait() and then resumes clean execution.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("[Parent PID %d] Forking a child process...\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process calls execl
        printf("[Child PID %d] Replaces itself with '/bin/echo'...\n", getpid());
        execl("/bin/echo", "echo", "[Exec Image] Child successfully executed /bin/echo!", NULL);

        // This line only prints if execl fails
        perror("execl failed in child");
        return 1;
    } else {
        // Parent waits for child
        int status;
        wait(&status);
        printf("[Parent PID %d] Child process %d finished execution.\n", getpid(), pid);
        printf("[Parent] Resuming parent code and exiting cleanly.\n");
    }

    return 0;
}
