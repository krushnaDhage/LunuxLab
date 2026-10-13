/*
 * File: 03_alarm_input_timeout.c
 * Description: Enforces a 10-second user input timeout using alarm() and SIGALRM.
 *              - Installs a signal handler for SIGALRM.
 *              - Sets alarm(10) before reading input.
 *              - If the user fails to provide input within 10 seconds, SIGALRM fires,
 *                the handler prints a timeout message, and terminates the program.
 *              - If the user enters input on time, alarm(0) cancels the pending alarm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define TIMEOUT_SECONDS 10

// Signal handler for SIGALRM
void sigalrm_handler(int sig) {
    printf("\n\n[TIMEOUT] Time limit of %d seconds exceeded!\n", TIMEOUT_SECONDS);
    printf("          No input received from user. Terminating program.\n");
    exit(1);
}

int main(void) {
    char user_buffer[100];

    // Register handler for SIGALRM
    signal(SIGALRM, sigalrm_handler);

    printf("==========================================================\n");
    printf("         INPUT TIMEOUT DEMONSTRATION (alarm)              \n");
    printf("==========================================================\n");
    printf("You have %d seconds to enter your name...\n\n", TIMEOUT_SECONDS);

    // Schedule SIGALRM signal in 10 seconds
    alarm(TIMEOUT_SECONDS);

    printf("Enter your name: ");
    fflush(stdout);

    if (fgets(user_buffer, sizeof(user_buffer), stdin) != NULL) {
        // Input received successfully: CANCEL THE ALARM
        alarm(0);
        printf("\n[SUCCESS] Input received within time limit!\n");
        printf("Hello, %s", user_buffer);
    }

    printf("Program completed normally.\n");
    return 0;
}
