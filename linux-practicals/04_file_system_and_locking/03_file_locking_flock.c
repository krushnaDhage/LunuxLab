/*
 * File: 03_file_locking_flock.c
 * Description: Demonstrates file locking using the flock() system call.
 *              Evaluates the assignment's stated criteria for mandatory locking:
 *              - Regular file
 *              - Set-Group-ID bit ON (S_ISGID)
 *              - Group-Execute bit OFF (S_IXGRP)
 *
 * Critical Linux Practical Notes:
 *  1. flock() in Linux is strictly ADVISORY: Cooperating processes honor
 *     the lock, but uncooperative processes can still read/write using standard open().
 *  2. The SGID-on / Group-Execute-off condition historically applied to fcntl()
 *     record locking on filesystems mounted with '-o mand'.
 *  3. Modern Linux kernels (5.15+) have deprecated and removed mandatory locking
 *     entirely because it causes kernel deadlocks and race conditions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>

void check_mandatory_locking_criteria(mode_t mode) {
    printf("\n--- Mandatory Locking Criteria Check (Assignment Rule) ---\n");
    int is_reg = S_ISREG(mode);
    int sgid_on = (mode & S_ISGID) ? 1 : 0;
    int grp_exec_off = !(mode & S_IXGRP) ? 1 : 0;

    printf("1. Regular File?                  : %s\n", is_reg ? "YES" : "NO");
    printf("2. Set-Group-ID (SGID) bit ON?     : %s\n", sgid_on ? "YES" : "NO");
    printf("3. Group-Execute (S_IXGRP) bit OFF?: %s\n", grp_exec_off ? "YES" : "NO");

    if (is_reg && sgid_on && grp_exec_off) {
        printf("-> Criteria met! In historical Unix/fcntl(), this designated mandatory locking.\n");
        printf("   However, flock() on modern Linux remains advisory unless mounted with '-o mand'.\n");
    } else {
        printf("-> Criteria NOT met: File locks operate strictly in ADVISORY mode.\n");
    }
}

int main(int argc, char *argv[]) {
    const char *filepath = (argc > 1) ? argv[1] : "lock_test.txt";

    // Create / open test file
    int fd = open(filepath, O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat failed");
        close(fd);
        return 1;
    }

    printf("==========================================================\n");
    printf("           FILE LOCKING DEMONSTRATION (flock)             \n");
    printf("==========================================================\n");
    printf("Target File: %s (Permissions: %04o)\n", filepath, st.st_mode & 07777);

    check_mandatory_locking_criteria(st.st_mode);

    // Apply Exclusive Lock using flock()
    printf("\nApplying EXCLUSIVE lock (LOCK_EX) using flock()...\n");
    if (flock(fd, LOCK_EX | LOCK_NB) == 0) {
        printf("[SUCCESS] File locked exclusively! (PID: %d holds the lock)\n", getpid());
        printf("Holding lock for 5 seconds... Try running another instance in Terminal 2.\n");
        sleep(5);

        // Unlock file
        printf("Releasing lock (LOCK_UN)...\n");
        flock(fd, LOCK_UN);
        printf("[UNLOCKED] Lock released successfully.\n");
    } else {
        perror("[FAILED] flock could not acquire lock (another process is holding it)");
    }

    close(fd);
    printf("==========================================================\n");
    return 0;
}
