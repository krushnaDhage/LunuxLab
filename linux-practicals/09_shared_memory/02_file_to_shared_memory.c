/*
 * File: 02_file_to_shared_memory.c
 * Description: Reads a small file and writes its content and size into a
 *              System V Shared Memory segment.
 * Usage: ./02_file_to_shared_memory [file_to_read]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_BUFFER 4096

// Shared memory data structure
struct file_shm {
    int ready;                // Flag: 1 when data is written
    size_t length;            // Number of bytes in content
    char data[MAX_BUFFER];    // File payload
};

int main(int argc, char *argv[]) {
    char filename[256];

    if (argc >= 2) {
        snprintf(filename, sizeof(filename), "%s", argv[1]);
    } else {
        printf("Enter small file path to copy into shared memory: ");
        if (scanf("%255s", filename) != 1) {
            printf("Invalid file path.\n");
            return 1;
        }
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File '%s' not found. Creating a sample file...\n", filename);
        fp = fopen(filename, "w+");
        if (fp) {
            fprintf(fp, "This is sample data stored inside Shared Memory.\n"
                        "Used for Linux System Programming practical experiment.\n");
            rewind(fp);
        } else {
            perror("Failed to create file");
            return 1;
        }
    }

    // Generate IPC key
    key_t key = ftok(".", 'S');
    if (key == -1) key = 54321;

    // Allocate shared memory segment
    int shmid = shmget(key, sizeof(struct file_shm), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        fclose(fp);
        return 1;
    }

    // Attach to shared memory
    struct file_shm *shm_ptr = (struct file_shm *)shmat(shmid, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat failed");
        fclose(fp);
        return 1;
    }

    // Read file contents into shared memory buffer
    shm_ptr->length = fread(shm_ptr->data, 1, MAX_BUFFER - 1, fp);
    shm_ptr->data[shm_ptr->length] = '\0';
    shm_ptr->ready = 1;

    fclose(fp);

    printf("==========================================================\n");
    printf("         FILE TO SHARED MEMORY (WRITER)                   \n");
    printf("==========================================================\n");
    printf("Source File      : %s\n", filename);
    printf("Shared Memory ID : %d\n", shmid);
    printf("Bytes Written    : %zu bytes\n", shm_ptr->length);
    printf("Status           : Ready for reader process.\n");
    printf("==========================================================\n");
    printf("Now run './03_shared_memory_to_file' to read this into a file.\n");

    // Detach shared memory
    shmdt(shm_ptr);
    return 0;
}
