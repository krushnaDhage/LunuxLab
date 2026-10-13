/*
 * File: 02_handle_ctrl_c.c
 * Description: Demonstrates trapping the Ctrl+C signal (SIGINT).
 *              When the user presses Ctrl+C, instead of terminating the program,
 *              the registered signal handler intercepts SIGINT, prints a message,
 *              and allows the program to continue executing.
 *              (To terminate cleanly during tests, press Ctrl+C 3 times or press Ctrl+\).
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static int sigint_count = 0;

void sigint_handler(int sig) {
    sigint_count++;
    printf("\n[Signal Handler] Trapped Ctrl+C (SIGINT: Signal %d)!\n", sig);
    printf("                 The program does NOT quit on Ctrl+C. (Count: %d/3)\n", sigint_count);

    if (sigint_count >= 3) {
        printf("                 Caught 3 Ctrl+C signals. Exiting cleanly now.\n");
        exit(0);
    } else {
        printf("                 Press Ctrl+C again or use Ctrl+\\ to force exit.\n\n");
    }
}

int main(void) {
    // Register signal handler for SIGINT
    signal(SIGINT, sigint_handler);

    printf("==========================================================\n");
    printf("         TRAPPING Ctrl+C (SIGINT) DEMONSTRATION           \n");
    printf("==========================================================\n");
    printf("Program running (PID: %d). Press Ctrl+C to test the handler.\n\n", getpid());

    while (1) {
        printf("Working quietly... (Press Ctrl+C)\n");
        sleep(2);
    }

    return 0;
}
