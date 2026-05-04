#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

// Global flag
volatile sig_atomic_t keep_running = 1;

// Signal handler
void handle_sigint(int sig) {
    keep_running = 0;
}

// Worker thread function
void* worker(void* arg) {
    int id = *(int*)arg;

    while (keep_running) {
        printf("Thread %d is running...\n", id);
        sleep(1);
    }

    printf("Thread %d exiting...\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    // Set up signal handler for Ctrl+C
    signal(SIGINT, handle_sigint);

    // Create threads
    pthread_create(&t1, NULL, worker, &id1);
    pthread_create(&t2, NULL, worker, &id2);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("All threads cleanly exited. Goodbye.\n");

    return 0;
}