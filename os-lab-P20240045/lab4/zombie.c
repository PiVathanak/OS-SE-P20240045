#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            printf("Child %d (PID: %d) sleeping...\n", i+1, getpid());
            sleep(20);
            printf("Child %d exiting\n", i+1);
            exit(0);
        }
    }

    // Parent waits for all children
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    printf("Parent: all children finished\n");

    return 0;
}