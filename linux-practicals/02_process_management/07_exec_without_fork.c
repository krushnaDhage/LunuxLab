/*
 * File: 07_exec_without_fork.c
 * Description: Demonstrates calling execl() without fork().
 *              When execl() succeeds, the current process image is completely
 *              overwritten, and code following execl() NEVER runs.
 *              execl() returns ONLY if an error occurs (returns -1).
 * Usage:
 *   ./07_exec_without_fork           (runs valid command: /bin/ls)
 *   ./07_exec_without_fork invalid   (runs invalid command to show error handling)
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("=== Process PID: %d ===\n", getpid());
    printf("About to call execl()...\n");

    if (argc > 1 && strcmp(argv[1], "invalid") == 0) {
        printf("Testing INVALID command: /bin/non_existent_command\n");
        int ret = execl("/bin/non_existent_command", "non_existent_command", NULL);
        
        // This will execute because the command fails
        fprintf(stderr, "[ERROR] execl() returned: %d, errno: %d (%s)\n", 
                ret, errno, strerror(errno));
        fprintf(stderr, "Because execl failed, this print statement STILL runs!\n");
    } else {
        printf("Testing VALID command: /bin/ls -l\n");
        printf("Note: If execl succeeds, the line below this will NEVER execute!\n\n");
        
        int ret = execl("/bin/ls", "ls", "-l", NULL);

        // If execl succeeds, control NEVER reaches here!
        fprintf(stderr, "[ERROR] This line should never print if execl succeeded! Return: %d\n", ret);
    }

    return 0;
}
