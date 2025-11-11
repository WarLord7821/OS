/*
 * C Program to demonstrate a pipe for Inter-Process Communication (IPC).
 * The child process reads a string from the user (stdin)
 * and sends it to the parent process through the pipe.
 * The parent process reads the string from the pipe and prints it.
 */

#include <stdio.h>    // For printf, scanf, fgets
#include <stdlib.h>   // For exit()
#include <unistd.h>   // For pipe(), fork(), read(), write(), close()
#include <string.h>   // For strlen()
#include <sys/wait.h> // For wait()

int main() {
    
    // STEP 1: Define variables
    int fd[2]; // File descriptor array for the pipe
               // fd[0] is the READ end
               // fd[1] is the WRITE end
               
    char write_buffer[100];
    char read_buffer[100];
    pid_t pid;

    // STEP 2: Create the pipe
    // pipe() returns 0 on success, -1 on error
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(1);
    }
    
    //  for read and fd[1] for write]

    // STEP 3: Fork the process to create a child
    pid = fork();

    if (pid < 0) { // Fork Failed
        perror("Fork failed");
        exit(1);
    }

    // STEP 4: Child Process Logic
    if (pid == 0) {
        printf("CHILD: Process started. Enter a string: ");
        
        // Read a string from the user
        fgets(write_buffer, sizeof(write_buffer), stdin);

        // A. Close the unused READ end of the pipe in the child
        close(fd[0]);

        // B. Write the string into the WRITE end of the pipe
        // We add +1 to strlen to include the null terminator '\0'
        printf("CHILD: Writing string to pipe...\n");
        write(fd[1], write_buffer, strlen(write_buffer) + 1);

        // C. Close the WRITE end after finishing
        close(fd[1]);
        
        exit(0); // Child process is done
    }
    
    // STEP 5: Parent Process Logic
    else {
        // A. Wait for the child process to finish.
        // This ensures the child has written to the pipe before the parent reads.
        wait(NULL); 

        // B. Close the unused WRITE end of the pipe in the parent
        close(fd[1]);

        // C. Read the string from the READ end of the pipe
        printf("PARENT: Reading string from pipe...\n");
        read(fd[0], read_buffer, sizeof(read_buffer));

        // D. Print the string
        printf("PARENT: Received string from child: %s\n", read_buffer);

        // E. Close the READ end after finishing
        close(fd[0]);
    }

    return 0;
}