/*
 * File: 02_fifo_receiver.c (Process B)
 * Description: Receives file contents sent through a FIFO (named pipe),
 *              stores them into an output file, and displays the total bytes transferred.
 * Usage: ./02_fifo_receiver [output_file]
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
    const char *dest_file = (argc > 1) ? argv[1] : "received_output.txt";

    // Ensure FIFO exists
    mkfifo(FIFO_NAME, 0666);

    printf("==========================================================\n");
    printf("         FIFO FILE RECEIVER (Process B)                   \n");
    printf("==========================================================\n");
    printf("Output File: %s\n", dest_file);
    printf("FIFO Pipe  : %s\n", FIFO_NAME);
    printf("Waiting for Sender (Process A) to connect...\n");

    // Open FIFO for reading (blocks until sender opens it for writing)
    int fifo_fd = open(FIFO_NAME, O_RDONLY);
    if (fifo_fd < 0) {
        perror("open FIFO for reading failed");
        return 1;
    }

    int out_fd = open(dest_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (out_fd < 0) {
        perror("open destination file failed");
        close(fifo_fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, total_received = 0;

    printf("\n[Receiver] Connected! Receiving data stream...\n");
    while ((bytes_read = read(fifo_fd, buffer, sizeof(buffer))) > 0) {
        write(out_fd, buffer, bytes_read);
        total_received += bytes_read;
    }

    close(out_fd);
    close(fifo_fd);

    // Clean up FIFO special file
    unlink(FIFO_NAME);

    printf("==========================================================\n");
    printf("[Receiver] Data transfer complete!\n");
    printf("           Total Bytes Received and Stored: %zd bytes\n", total_received);
    printf("           Stored File                    : %s\n", dest_file);
    printf("==========================================================\n");

    return 0;
}
