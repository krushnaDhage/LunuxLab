/*
 * File: 02_linux_file_types.c
 * Description: Demonstrates detection of all 7 standard Linux file types:
 *              1. Regular File (S_ISREG)
 *              2. Directory (S_ISDIR)
 *              3. Character Device (S_ISCHR)
 *              4. Block Device (S_ISBLK)
 *              5. FIFO / Named Pipe (S_ISFIFO)
 *              6. Symbolic Link (S_ISLNK)
 *              7. Socket (S_ISSOCK)
 *
 * Uses lstat() so that symbolic links themselves are inspected
 * rather than their targets.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void inspect_path(const char *path) {
    struct stat st;

    if (lstat(path, &st) < 0) {
        printf("%-30s : [NOT ACCESSIBLE / DOES NOT EXIST]\n", path);
        return;
    }

    printf("%-30s : ", path);
    if (S_ISREG(st.st_mode)) {
        printf("Regular File (-)\n");
    } else if (S_ISDIR(st.st_mode)) {
        printf("Directory (d)\n");
    } else if (S_ISCHR(st.st_mode)) {
        printf("Character Device (c)\n");
    } else if (S_ISBLK(st.st_mode)) {
        printf("Block Device (b)\n");
    } else if (S_ISFIFO(st.st_mode)) {
        printf("FIFO / Named Pipe (p)\n");
    } else if (S_ISLNK(st.st_mode)) {
        printf("Symbolic Link (l)\n");
    } else if (S_ISSOCK(st.st_mode)) {
        printf("Socket (s)\n");
    } else {
        printf("Unknown Type (?)\n");
    }
}

int main(int argc, char *argv[]) {
    printf("==========================================================\n");
    printf("            LINUX FILE TYPES DEMONSTRATION                \n");
    printf("==========================================================\n");

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            inspect_path(argv[i]);
        }
    } else {
        printf("Demonstrating common standard Linux paths for all 7 types:\n\n");
        inspect_path("/etc/passwd");            // Regular file
        inspect_path("/home");                  // Directory
        inspect_path("/dev/null");              // Character device
        inspect_path("/dev/sda");               // Block device (or loop0 / sdb)
        inspect_path("/bin");                   // Symbolic link (on Ubuntu /bin -> usr/bin)
        inspect_path("/dev/log");               // Socket (or systemd socket)
        inspect_path("/run/systemd/journal/stdout"); // Socket or FIFO
    }

    printf("==========================================================\n");
    return 0;
}
