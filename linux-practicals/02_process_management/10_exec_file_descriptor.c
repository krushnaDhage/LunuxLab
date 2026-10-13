/*
 * File: 10_exec_file_descriptor.c
 * Description: Demonstrates file descriptor inheritance across exec().
 *              File descriptors remain open across exec() by default (unless
 *              FD_CLOEXEC is explicitly set).
 *
 * Flow:
 *  1. Initial run: Opens a file with open() getting file descriptor 'fd'.
 *  2. Forks a child process.
 *  3. Child converts 'fd' into a string and passes it as an argument to execl(),
 *     re-executing this binary in "child-exec" mode.
 *  4. After execl(), the program retrieves 'fd' from argv[1] and writes
 *     to the file using write() WITHOUT calling open() again.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        // --- Post-exec Phase (Running inside the new process image) ---
        int inherited_fd = atoi(argv[1]);
        printf("[Post-Exec Process PID %d] Running with inherited file descriptor: %d\n", 
               getpid(), inherited_fd);

        const char *msg = "Success: This text was written using inherited FD across execl()!\n";
        ssize_t bytes_written = write(inherited_fd, msg, strlen(msg));

        if (bytes_written < 0) {
            perror("write to inherited_fd failed");
            return 1;
        }

        printf("[Post-Exec Process] Wrote %zd bytes to inherited FD successfully without reopening!\n", 
               bytes_written);
        close(inherited_fd);
        return 0;
    }

    // --- Pre-exec Phase (Parent / Child setup) ---
    const char *test_file = "exec_fd_test.txt";
    printf("[Parent PID %d] Opening file '%s'...\n", getpid(), test_file);

    int fd = open(test_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }
    printf("[Parent] Opened file. File Descriptor = %d\n", fd);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        close(fd);
        return 1;
    } else if (pid == 0) {
        // Child: converts fd to string and calls execl()
        char fd_str[16];
        snprintf(fd_str, sizeof(fd_str), "%d", fd);

        printf("[Child PID %d] Calling execl() passing fd '%s'...\n", getpid(), fd_str);
        execl(argv[0], argv[0], fd_str, NULL);

        // Reached only if execl fails
        perror("execl failed");
        exit(1);
    } else {
        // Parent waits for the executed child
        wait(NULL);
        close(fd);
        printf("[Parent] Child finished. Check contents of '%s':\n", test_file);
    }

    return 0;
}
