#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t running = 1;

void handle_signal(int sig) {
    printf("\nCaught signal %d, cleaning up...\n", sig);
    running = 0;
}

int main(void) {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("PID %d running. Send SIGINT (Ctrl+C) or SIGTERM to stop.\n", getpid());
    while (running) {
        sleep(1);
    }
    printf("Cleanup complete, exiting.\n");
    return 0;
}