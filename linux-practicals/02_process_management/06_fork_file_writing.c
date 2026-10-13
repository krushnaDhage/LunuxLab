/*
 * File: 06_fork_file_writing.c
 * Description: Demonstrates file descriptor sharing after fork().
 *              When a file is opened before fork(), both parent and child share
 *              the same open file description, including the file offset.
 *              Writes from parent and child append one after the other rather
 *              than overwriting, but order depends on process scheduling.
 *              This program tests writing both with and without newline characters.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    const char *filename_no_nl = "output_no_newline.txt";
    const char *filename_with_nl = "output_with_newline.txt";

    printf("=== Experiment 1: Writing WITHOUT newlines ===\n");
    int fd1 = open(filename_no_nl, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd1 < 0) {
        perror("open failed");
        return 1;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        const char *msg_child = "From Child ";
        write(fd1, msg_child, strlen(msg_child));
        close(fd1);
        exit(0);
    } else {
        const char *msg_parent = "From Parent ";
        write(fd1, msg_parent, strlen(msg_parent));
        wait(NULL);
        close(fd1);
    }

    printf("Finished writing to '%s'.\n", filename_no_nl);

    printf("\n=== Experiment 2: Writing WITH newlines ===\n");
    int fd2 = open(filename_with_nl, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd2 < 0) {
        perror("open failed");
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        const char *msg_child_nl = "From Child\n";
        write(fd2, msg_child_nl, strlen(msg_child_nl));
        close(fd2);
        exit(0);
    } else {
        const char *msg_parent_nl = "From Parent\n";
        write(fd2, msg_parent_nl, strlen(msg_parent_nl));
        wait(NULL);
        close(fd2);
    }

    printf("Finished writing to '%s'.\n", filename_with_nl);
    printf("Check both files using 'cat output_no_newline.txt' and 'cat output_with_newline.txt'\n");

    return 0;
}
