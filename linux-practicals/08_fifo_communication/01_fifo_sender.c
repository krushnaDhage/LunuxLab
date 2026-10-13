/*
 * File: 01_fifo_sender.c (Process A)
 * Description: Reads a text file and transfers its entire content through a FIFO (named pipe).
 *              Displays total bytes transferred.
 * Usage: ./01_fifo_sender [source_file]
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_NAME "my_test_fifo"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    const char *source_file = (argc > 1) ? argv[1] : "sample_source.txt";

    // If source file does not exist, create a sample one for convenient testing
    FILE *check_fp = fopen(source_file, "r");
    if (!check_fp) {
        printf("[Sender] Creating sample source file '%s'...\n", source_file);
        FILE *create_fp = fopen(source_file, "w");
        if (create_fp) {
            fprintf(create_fp, "Hello from Linux FIFO transfer!\n"
                               "This is line 2 of the demonstration text file.\n"
                               "Linux System Programming practical exam test data.\n");
            fclose(create_fp);
        }
    } else {
        fclose(check_fp);
    }

    // Create the FIFO if it doesn't already exist
    mkfifo(FIFO_NAME, 0666);

    printf("==========================================================\n");
    printf("         FIFO FILE SENDER (Process A)                     \n");
    printf("==========================================================\n");
    printf("Source File: %s\n", source_file);
    printf("FIFO Pipe  : %s\n", FIFO_NAME);
    printf("Waiting for Receiver (Process B) to open the FIFO...\n");

    // Open FIFO for writing (blocks until receiver opens it for reading)
    int fifo_fd = open(FIFO_NAME, O_WRONLY);
    if (fifo_fd < 0) {
        perror("open FIFO for writing failed");
        return 1;
    }

    int file_fd = open(source_file, O_RDONLY);
    if (file_fd < 0) {
        perror("open source file failed");
        close(fifo_fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, total_sent = 0;

    printf("\n[Sender] Connected! Transferring file content...\n");
    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_written = write(fifo_fd, buffer, bytes_read);
        if (bytes_written > 0) {
            total_sent += bytes_written;
        }
    }

    close(file_fd);
    close(fifo_fd);

    printf("==========================================================\n");
    printf("[Sender] Transfer completed successfully!\n");
    printf("         Total Bytes Sent: %zd bytes\n", total_sent);
    printf("==========================================================\n");

    return 0;
}
