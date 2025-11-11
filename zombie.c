/*
 * C Program to create a ZOMBIE process
 */

#include <stdio.h>
#include <stdlib.h>   // For exit()
#include <unistd.h>   // For fork(), sleep()
#include <sys/types.h> // For pid_t

int main() {
    
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // --- CHILD PROCESS ---
        printf("CHILD: I am the child (PID: %d).\n", getpid());
        printf("CHILD: I am exiting now.\n");
        exit(0); // The child process exits immediately.
    }
    else {
        // --- PARENT PROCESS ---
        printf("PARENT: I am the parent (PID: %d).\n", getpid());
        printf("PARENT: I am sleeping for 30 seconds.\n");
        
        // The parent does NOT call wait().
        // It just sleeps, giving you time to check.
        sleep(30);

        printf("PARENT: I am awake now and exiting.\n");
        // The zombie child will be "reaped" by the OS when the parent finally exits.
    }

    return 0;
}


//run the code then immediately run the command 'ps -el | grep Z' in another terminal to see the zombie process.