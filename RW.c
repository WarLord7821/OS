/*
 * C Program for the Readers-Writers Problem
 * (Readers-Priority Solution)
 *
 * This program uses pthreads and POSIX semaphores.
 */

// STEP 1: Include all necessary libraries
#include <stdio.h>
#include <pthread.h>  // For creating and managing threads
#include <semaphore.h> // For using semaphores (the locks)
#include <unistd.h>  // For using the sleep() function
#include <stdlib.h>  // For exit()

// STEP 2: Define global semaphores and shared variables
sem_t wrt;            // Semaphore for "write" access. Blocks writers AND first reader.
sem_t mutex;          // Semaphore for "mutex" (mutual exclusion)
                      // Its ONLY job is to protect the 'read_count' variable.
int read_count = 0;   // Counts how many readers are currently in the critical section.
int shared_data = 1;  // The shared resource we are reading/writing.

// --- Function Prototypes ---
void *writer(void *arg);
void *reader(void *arg);

// STEP 5: The main() function
int main() {
    int num_readers, num_writers;

    // Get user input for the number of threads
    printf("Enter number of Readers: ");
    scanf("%d", &num_readers);
    printf("Enter number of Writers: ");
    scanf("%d", &num_writers);

    // Arrays to hold the thread identifiers
    pthread_t reader_threads[num_readers];
    pthread_t writer_threads[num_writers];

    // Arrays to hold the simple IDs (1, 2, 3...) for our threads
    int reader_ids[num_readers];
    int writer_ids[num_writers];

    // STEP 6: Initialize the semaphores
    // sem_init(&semaphore_name, 0 (share between threads), initial_value);
    
    // 'wrt' starts at 1 (unlocked), allowing one Writer or the first Reader.
    if (sem_init(&wrt, 0, 1) == -1) {
        perror("sem_init wrt failed");
        exit(1);
    }
    // 'mutex' starts at 1 (unlocked), protecting 'read_count'.
    if (sem_init(&mutex, 0, 1) == -1) {
        perror("sem_init mutex failed");
        exit(1);
    }

    printf("\n--- Simulation Starting ---\n");

    // STEP 7: Create all the Writer threads
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1; // Give it a simple ID (1, 2, ...)
        // Create the thread, passing it the writer() function and its ID
        if (pthread_create(&writer_threads[i], NULL, writer, &writer_ids[i]) != 0) {
            perror("pthread_create writer failed");
        }
    }

    // STEP 8: Create all the Reader threads
    for (int i = 0; i < num_readers; i++) {
        reader_ids[i] = i + 1; // Give it a simple ID (1, 2, ...)
        // Create the thread, passing it the reader() function and its ID
        if (pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]) != 0) {
            perror("pthread_create reader failed");
        }
    }

    // STEP 9: Wait for all threads to finish (Join)
    // We join writers first, then readers.
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writer_threads[i], NULL);
    }
    for (int i = 0; i < num_readers; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    printf("\n--- Simulation Finished ---\n");

    // STEP 10: Clean up and destroy the semaphores
    sem_destroy(&wrt);
    sem_destroy(&mutex);

    return 0;
}

// STEP 3: The Writer Thread Function
// This function will be executed by all writer threads.
void *writer(void *arg) {
    int writer_id = *(int *)arg; // Get the writer's ID

    // Simulate the writer working, then trying to access the resource
    sleep(rand() % 3); 
    printf("[Writer %d] is trying to write.\n", writer_id);

    // --- Entry Section ---
    // A writer must wait for the 'wrt' lock.
    // If a reader (or another writer) has it, it will wait here.
    sem_wait(&wrt);

    // --- Critical Section ---
    // The writer has the lock, no one else can be here.
    printf(">>> [Writer %d] is WRITING... <<<\n", writer_id);
    shared_data++; // Modify the shared data
    sleep(1);      // Simulate the time it takes to write
    printf(">>> [Writer %d] finished. Shared data is now: %d <<<\n", writer_id, shared_data);
    
    // --- Exit Section ---
    // Release the 'wrt' lock, allowing others to enter.
    sem_post(&wrt);

    return NULL;
}

// STEP 4: The Reader Thread Function
// This function will be executed by all reader threads.
void *reader(void *arg) {
    int reader_id = *(int *)arg; // Get the reader's ID

    // Simulate the reader working, then trying to access the resource
    sleep(rand() % 3);
    printf("[Reader %d] is trying to read.\n", reader_id);

    // --- Entry Section ---
    // 1. Lock 'mutex' to protect 'read_count'
    sem_wait(&mutex);
    read_count++; // Increment the count of readers

    // 2. Check if this is the FIRST reader
    if (read_count == 1) {
        // If it is, it must also grab the 'wrt' lock
        // This blocks any waiting WRITERS from entering
        printf("[Reader %d] is the first reader, locking 'wrt' for writers.\n", reader_id);
        sem_wait(&wrt);
    }

    // 3. Release 'mutex'.
    // This is vital! It allows OTHER READERS to enter
    // while this first reader keeps 'wrt' locked.
    sem_post(&mutex);

    // --- Critical Section ---
    // Multiple readers can be in this section at the same time.
    printf("[Reader %d] is READING. (Total Readers: %d). Shared data is: %d\n", 
           reader_id, read_count, shared_data);
    sleep(1); // Simulate the time it takes to read
    
    // --- Exit Section ---
    // 4. Lock 'mutex' again to protect 'read_count'
    sem_wait(&mutex);
    read_count--; // Decrement the count of readers

    // 5. Check if this is the LAST reader
    if (read_count == 0) {
        // If it is, it must release the 'wrt' lock,
        // allowing a waiting WRITER to finally enter.
        printf("[Reader %d] is the last reader, unlocking 'wrt' for writers.\n", reader_id);
        sem_post(&wrt);
    }

    // 6. Release 'mutex'
    sem_post(&mutex);
    
    printf("[Reader %d] has finished reading.\n", reader_id);

    return NULL;
}