/* forkchild.c — Process creation with fork() and exec() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("Parent process (PID: %d) — creating child...\n", getpid());

    // 1. Create the child process
    pid = fork();

    if (pid < 0) {
        /* fork failed */
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        /* ---- CHILD PROCESS ---- */
        // Inside the child, fork() returns 0
        printf("Child process (PID: %d, Parent PID: %d)\n", getpid(), getppid());

        /*
         * Sleep for 30 seconds so you have time to observe
         * the parent-child process tree in another terminal.
         */
        printf("Child: sleeping 30s — observe the process tree now!\n");
        sleep(30);

        printf("Child: now executing 'ls -la'...\n");

        /* 2. Replace this process with "ls -la" 
         * execlp(file, arg0, arg1, ..., NULL)
         */
        execlp("ls", "ls", "-la", NULL);

        /* If execlp returns, it means it failed */
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else {
        /* ---- PARENT PROCESS ---- */
        // Inside the parent, fork() returns the PID of the child
        int status;
        printf("Parent: waiting for child (PID: %d) to finish...\n", pid);

        /* 3. Wait for the specific child process to terminate */
        waitpid(pid, &status, 0);

        /* 4. Check if the child exited normally */
        if (WIFEXITED(status)) {
            printf("Parent: child exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Parent: child terminated abnormally\n");
        }
        
        printf("Parent: done.\n");
    }

    return 0;
}