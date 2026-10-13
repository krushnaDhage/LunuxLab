/*
 * File: 04_interprocess_signal_communication.c
 * Description: Communicates between two independent processes running in
 *              separate terminals using kill() and signal().
 *
 * How to run across two terminals:
 *  Terminal 1:
 *    ./04_interprocess_signal_communication
 *    (Note the printed PID, e.g., 24501)
 *
 *  Terminal 2:
 *    ./04_interprocess_signal_communication
 *    (Enter the PID from Terminal 1 and choose signals to send)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Signal handlers for incoming signals
void handle_sigusr1(int sig) {
    printf("\n[Incoming Signal] Received SIGUSR1 (Signal %d): Ping / Hello from Peer!\n", sig);
    printf("Choice > ");
    fflush(stdout);
}

void handle_sigusr2(int sig) {
    printf("\n[Incoming Signal] Received SIGUSR2 (Signal %d): Acknowledgment from Peer!\n", sig);
    printf("Choice > ");
    fflush(stdout);
}

void handle_sigterm(int sig) {
    printf("\n[Incoming Signal] Received SIGTERM (Signal %d): Termination requested by Peer. Exiting!\n", sig);
    exit(0);
}

int main(void) {
    // Install signal handlers
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);
    signal(SIGTERM, handle_sigterm);

    pid_t my_pid = getpid();
    pid_t target_pid = 0;
    int choice = 0;

    printf("==========================================================\n");
    printf("     INTERPROCESS SIGNAL COMMUNICATION (TWO TERMINALS)    \n");
    printf("==========================================================\n");
    printf(" My Process PID: %d\n", my_pid);
    printf(" Open another terminal and run this program there.\n");
    printf("==========================================================\n");

    printf("Enter target process PID to communicate with: ");
    if (scanf("%d", &target_pid) != 1) {
        printf("Invalid PID.\n");
        return 1;
    }

    while (1) {
        printf("\n--- Send Signal to PID %d ---\n", target_pid);
        printf("1. Send SIGUSR1 (Ping)\n");
        printf("2. Send SIGUSR2 (Ack)\n");
        printf("3. Send SIGTERM (Terminate Peer)\n");
        printf("4. Exit this program\n");
        printf("Choice > ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                if (kill(target_pid, SIGUSR1) == 0)
                    printf("[Sent] Sent SIGUSR1 to PID %d\n", target_pid);
                else
                    perror("kill failed");
                break;
            case 2:
                if (kill(target_pid, SIGUSR2) == 0)
                    printf("[Sent] Sent SIGUSR2 to PID %d\n", target_pid);
                else
                    perror("kill failed");
                break;
            case 3:
                if (kill(target_pid, SIGTERM) == 0)
                    printf("[Sent] Sent SIGTERM to PID %d\n", target_pid);
                else
                    perror("kill failed");
                break;
            case 4:
                printf("Exiting.\n");
                return 0;
            default:
                printf("Invalid option.\n");
                break;
        }
    }

    return 0;
}
