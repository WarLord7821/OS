/*
 * C Program to create an ORPHAN process
 */

#include <stdio.h>
#include <stdlib.h>   // For exit()
#include <unistd.h>   // For fork(), sleep(), getpid(), getppid()
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
        printf("CHILD: My parent's PID is: %d\n", getppid());
        
        printf("CHILD: I am sleeping for 5 seconds...\n");
        sleep(5); // Sleep to let the parent exit first
        
        printf("CHILD: I am awake.\n");
        printf("CHILD: My parent's PID is NOW: %d\n", getppid());
        printf("CHILD: I am an orphan, adopted by init (PID 1). Exiting.\n");
    }
    else {
        // --- PARENT PROCESS ---
        printf("PARENT: I am the parent (PID: %d).\n", getpid());
        printf("PARENT: My child's PID is: %d\n", pid);
        printf("PARENT: I am exiting NOW.\n");
        exit(0); // The parent exits immediately, orphaning the child.
    }

    return 0;
}