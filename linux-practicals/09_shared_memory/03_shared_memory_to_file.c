/*
 * File: 03_shared_memory_to_file.c
 * Description: Reads content and length from System V Shared Memory
 *              and writes it into an output destination file.
 *              Cleans up and removes the shared memory segment.
 * Usage: ./03_shared_memory_to_file [output_file]
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_BUFFER 4096

// Shared memory data structure
struct file_shm {
    int ready;
    size_t length;
    char data[MAX_BUFFER];
};

int main(int argc, char *argv[]) {
    const char *out_filename = (argc > 1) ? argv[1] : "recovered_from_shm.txt";

    // Generate matching IPC key
    key_t key = ftok(".", 'S');
    if (key == -1) key = 54321;

    // Connect to existing shared memory segment
    int shmid = shmget(key, sizeof(struct file_shm), 0666);
    if (shmid < 0) {
        printf("[Error] Shared memory not found. Run './02_file_to_shared_memory' first!\n");
        perror("shmget failed");
        return 1;
    }

    // Attach to shared memory
    struct file_shm *shm_ptr = (struct file_shm *)shmat(shmid, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat failed");
        return 1;
    }

    if (!shm_ptr->ready) {
        printf("[Warning] Shared memory segment exists but writer has not marked it ready.\n");
    }

    // Open destination file and write data
    FILE *fp = fopen(out_filename, "w");
    if (!fp) {
        perror("Failed to open output file");
        shmdt(shm_ptr);
        return 1;
    }

    size_t written = fwrite(shm_ptr->data, 1, shm_ptr->length, fp);
    fclose(fp);

    printf("==========================================================\n");
    printf("         SHARED MEMORY TO FILE (READER)                   \n");
    printf("==========================================================\n");
    printf("Shared Memory ID : %d\n", shmid);
    printf("Bytes Read       : %zu bytes\n", shm_ptr->length);
    printf("Output File      : %s\n", out_filename);
    printf("Bytes Saved      : %zu bytes\n", written);
    printf("==========================================================\n");

    // Detach and clean up segment
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    printf("Cleaned up and destroyed Shared Memory segment %d.\n", shmid);

    return 0;
}
