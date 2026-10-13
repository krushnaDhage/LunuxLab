/*
 * File: 05_fork_variable_behavior.c
 * Description: Demonstrates independent memory spaces after fork().
 *              Modifications to global and local variables in the child process
 *              do not affect the parent process, demonstrating that each process
 *              has its own distinct address space (Copy-on-Write).
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Global variable
int global_var = 100;

int main(void) {
    // Local variable
    int local_var = 50;

    printf("=== Before fork() ===\n");
    printf("Initial Values -> Global: %d, Local: %d\n\n", global_var, local_var);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child Process
        printf("[Child PID %d] Values before modification -> Global: %d, Local: %d\n", 
               getpid(), global_var, local_var);
        
        // Modifying variables in child
        global_var += 20;
        local_var += 20;

        printf("[Child PID %d] Values after modification  -> Global: %d, Local: %d\n\n", 
               getpid(), global_var, local_var);
    } else {
        // Parent Process waits for child to finish modifying first
        wait(NULL);

        printf("[Parent PID %d] Values in parent (after child finished) -> Global: %d, Local: %d\n", 
               getpid(), global_var, local_var);

        // Modifying variables in parent
        global_var += 500;
        local_var += 500;

        printf("[Parent PID %d] Values after parent modification     -> Global: %d, Local: %d\n", 
               getpid(), global_var, local_var);
    }

    return 0;
}
