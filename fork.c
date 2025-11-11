/*
 * C Program to demonstrate the basic use of fork()
 */

#include <stdio.h>    // For printf
#include <unistd.h>   // For fork(), getpid()
#include <sys/types.h> // For pid_t
#include <sys/wait.h> // For wait()

int main() {
    
    // pid_t is a special integer type for process IDs
    pid_t pid;

    // STEP 1: Create a new process
    pid = fork();

    // STEP 2: Check the return value of fork()
    if (pid < 0) {
        // CASE 1: Fork failed
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // CASE 2: We are in the CHILD process
        printf("--- CHILD PROCESS ---\n");
        printf("I am the child process.\n");
        printf("My PID is: %d\n", getpid());
        printf("My Parent's PID is: %d\n", getppid());
    }
    else {
        // CASE 3: We are in the PARENT process
        
        // Parent waits for the child to finish executing
        wait(NULL);
        
        printf("--- PARENT PROCESS ---\n");
        printf("I am the parent process.\n");
        printf("My PID is: %d\n", getpid());
        printf("My Child's PID is: %d\n", pid);
    }

    printf("--- COMMON PART ---\n");
    printf("This line is printed by BOTH parent (PID: %d) and child (PID: %d).\n", getpid(), pid);

    return 0;
}