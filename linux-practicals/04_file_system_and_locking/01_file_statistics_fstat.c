/*
 * File: 01_file_statistics_fstat.c
 * Description: Displays file statistics using the fstat() system call.
 *              Prompts user for a file or directory path, opens it to obtain
 *              a file descriptor, and calls fstat().
 *              Prints ONLY the required attributes:
 *              1. Inode Number
 *              2. User ID (UID)
 *              3. Group ID (GID)
 *              4. File Access Permissions (FAP)
 *              5. File Type
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void print_file_type(mode_t mode) {
    if (S_ISREG(mode))       printf("Regular File\n");
    else if (S_ISDIR(mode))  printf("Directory\n");
    else if (S_ISCHR(mode))  printf("Character Device\n");
    else if (S_ISBLK(mode))  printf("Block Device\n");
    else if (S_ISFIFO(mode)) printf("FIFO / Named Pipe\n");
    else if (S_ISLNK(mode))  printf("Symbolic Link\n");
    else if (S_ISSOCK(mode)) printf("Socket\n");
    else                     printf("Unknown\n");
}

int main(int argc, char *argv[]) {
    char filepath[256];
    struct stat st;

    if (argc >= 2) {
        snprintf(filepath, sizeof(filepath), "%s", argv[1]);
    } else {
        printf("Enter file or directory path: ");
        if (scanf("%255s", filepath) != 1) {
            fprintf(stderr, "Invalid input.\n");
            return 1;
        }
    }

    // open() required to get a file descriptor for fstat()
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    // Call fstat() on the file descriptor
    if (fstat(fd, &st) < 0) {
        perror("fstat failed");
        close(fd);
        return 1;
    }

    close(fd);

    printf("\n==========================================================\n");
    printf("               FILE STATISTICS (fstat)                    \n");
    printf("==========================================================\n");
    printf(" Target Path : %s\n", filepath);
    printf(" 1. Inode    : %lu\n", (unsigned long)st.st_ino);
    printf(" 2. UID      : %u\n", (unsigned int)st.st_uid);
    printf(" 3. GID      : %u\n", (unsigned int)st.st_gid);
    printf(" 4. FAP      : %04o (Octal Permissions)\n", st.st_mode & 0777);
    printf(" 5. File Type: ");
    print_file_type(st.st_mode);
    printf("==========================================================\n");

    return 0;
}
