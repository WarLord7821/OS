/**
 * Dynamic Banker's Algorithm Implementation in C
 *
 * This program implements the Banker's Algorithm for deadlock avoidance.
 * The number of processes and resources are not fixed and will be
 * determined by user input at runtime.
 *
 * It uses dynamic memory allocation (malloc, free) to manage the
 * Available, Max, Allocation, and Need data structures.
 */

#include <stdio.h>
#include <stdlib.h> // For malloc, free, and exit
#include <stdbool.h> // For bool, true, and false

// --- Global Variables ---
// We use global *pointers* for the main data structures.
// These will be "pointed" to memory blocks allocated by malloc() in main.

int *Available;   // 1D Array (vector)
int **Max;        // 2D Array (matrix)
int **Allocation; // 2D Array (matrix)
int **Need;       // 2D Array (matrix)

int numProcesses; // Total number of processes
int numResources; // Total number of resource types

// --- Function Prototypes ---
void allocateMemory();
void freeMemory();
void getUserInput();
void calculateNeedMatrix();
bool isSafe(int safeSequence[]);
bool resourceRequest(int processID, int request[]);
void printState();

/**
 * @brief Main function to drive the Banker's Algorithm simulation.
 */
int main() {
    // --- 1. Get Initial Sizes ---
    printf("--- Banker's Algorithm (Dynamic) ---\n");
    printf("Enter total number of processes: ");
    scanf("%d", &numProcesses);
    
    printf("Enter total number of resource types: ");
    scanf("%d", &numResources);

    // --- 2. Allocate Memory ---
    // Now that we have the sizes, we can allocate memory from the heap.
    allocateMemory();

    // --- 3. Get System State from User ---
    getUserInput();

    // --- 4. Calculate Need Matrix ---
    // Need = Max - Allocation
    calculateNeedMatrix();

    // --- 5. Print Initial State ---
    printf("\n### System Initial State ###\n");
    printState();

    // --- 6. Check Initial Safety ---
    printf("\n### Running Safety Algorithm on Initial State ###\n");
    
    // We can use a VLA (Variable Length Array) here,
    // as numProcesses is now known.
    int safeSequence[numProcesses]; 
    
    if (isSafe(safeSequence)) {
        printf("SUCCESS: System is in a SAFE state.\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < numProcesses; i++) {
            printf("P%d", safeSequence[i]);
            if (i < numProcesses - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("FAILURE: System is in an UNSAFE state.\n");
    }

    // --- 7. Interactive Request Loop ---
    printf("\n----------------------------------------------\n");
    printf("### Resource Request Simulation ###\n");
    
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        int pid;
        // Use a VLA for the request vector
        int request[numResources];

        printf("\nDo you want to make a resource request? (y/n): ");
        scanf(" %c", &choice);
        
        if (choice != 'y' && choice != 'Y') {
            break;
        }

        printf("Enter process ID (0 to %d) making the request: ", numProcesses - 1);
        scanf("%d", &pid);

        if (pid < 0 || pid >= numProcesses) {
            printf("Invalid process ID. Please try again.\n");
            continue;
        }

        printf("Enter the request vector for P%d (e.g., '1 0 2'): ", pid);
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &request[j]);
        }

        // Call the resource-request algorithm
        resourceRequest(pid, request);

        // Print the state after the attempt
        printf("\nCurrent system state:\n");
        printState();
    }

    // --- 8. Clean up ---
    // Free all the memory we allocated with malloc()
    freeMemory();

    printf("\nExiting program.\n");
    return 0;
}

/**
 * @brief Allocates memory for all global data structures based on
 * numProcesses and numResources.
 */
void allocateMemory() {
    // Allocate 1D array for Available
    Available = (int *)malloc(numResources * sizeof(int));

    // Allocate 2D arrays (array of pointers)
    Max = (int **)malloc(numProcesses * sizeof(int *));
    Allocation = (int **)malloc(numProcesses * sizeof(int *));
    Need = (int **)malloc(numProcesses * sizeof(int *));

    // Check for malloc failure (simplified check)
    if (Available == NULL || Max == NULL || Allocation == NULL || Need == NULL) {
        printf("Error: Memory allocation failed!\n");
        exit(1);
    }

    // Allocate memory for each row (each process's vector)
    for (int i = 0; i < numProcesses; i++) {
        Max[i] = (int *)malloc(numResources * sizeof(int));
        Allocation[i] = (int *)malloc(numResources * sizeof(int));
        Need[i] = (int *)malloc(numResources * sizeof(int));
        
        if (Max[i] == NULL || Allocation[i] == NULL || Need[i] == NULL) {
            printf("Error: Row memory allocation failed!\n");
            exit(1);
        }
    }
}

/**
 * @brief Frees all dynamically allocated memory.
 */
void freeMemory() {
    // Free each row in the 2D arrays
    for (int i = 0; i < numProcesses; i++) {
        free(Max[i]);
        free(Allocation[i]);
        free(Need[i]);
    }

    // Free the "array of pointers"
    free(Max);
    free(Allocation);
    free(Need);

    // Free the 1D array
    free(Available);
}

/**
 * @brief Prompts the user to fill in the initial system state.
 * (Total Resources, Allocation Matrix, Max Matrix).
 * It then calculates the initial Available vector.
 */
void getUserInput() {
    // We need a temporary 1D array to hold total resources
    int *totalResources = (int *)malloc(numResources * sizeof(int));
    if (totalResources == NULL) {
        printf("Error: Failed to allocate memory for totalResources\n");
        exit(1);
    }

    printf("\n--- Enter System State ---\n");

    // 1. Get Total Resources
    printf("Enter total instances for each resource:\n");
    for (int j = 0; j < numResources; j++) {
        printf("R%d: ", j);
        scanf("%d", &totalResources[j]);
    }

    // 2. Get Allocation Matrix
    printf("\nEnter the Allocation Matrix (%d processes x %d resources):\n", numProcesses, numResources);
    printf("     ");
    for(int j=0; j<numResources; j++) printf("R%d ", j);
    printf("\n");
    
    for (int i = 0; i < numProcesses; i++) {
        printf("P%d:  ", i);
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &Allocation[i][j]);
        }
    }

    // 3. Get Max Matrix
    printf("\nEnter the Max Matrix (%d processes x %d resources):\n", numProcesses, numResources);
    printf("     ");
    for(int j=0; j<numResources; j++) printf("R%d ", j);
    printf("\n");
    
    for (int i = 0; i < numProcesses; i++) {
        printf("P%d:  ", i);
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &Max[i][j]);
        }
    }

    // 4. Calculate Initial Available Vector
    // Available = Total - (Sum of all Allocations)
    
    // We can use a VLA here for a temporary sum
    int totalAllocated[numResources];
    for (int j = 0; j < numResources; j++) {
        totalAllocated[j] = 0; // Initialize sum to zero
    }
    
    // Sum up the allocations
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            totalAllocated[j] += Allocation[i][j];
        }
    }

    // Calculate Available
    for (int j = 0; j < numResources; j++) {
        Available[j] = totalResources[j] - totalAllocated[j];
    }
    
    // Free the temporary array for total resources
    free(totalResources);
}

/**
 * @brief Calculates the 'Need' matrix based on 'Max' and 'Allocation'.
 * Need = Max - Allocation
 * This function assumes Max and Allocation have been filled.
 */
void calculateNeedMatrix() {
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            // Need is the max a process *might* want minus what it *already* has.
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
}

/**
 * @brief Implements the Safety Algorithm.
 * Checks if the system is in a safe state.
 * @param safeSequence An array to be filled with the safe sequence if one is found.
 * @return true if the system is safe, false otherwise.
 */
bool isSafe(int safeSequence[]) {
    // --- Step 1: Initialize ---

    // 'Work' vector, a temporary copy of 'Available'.
    // Use VLA since numResources is known.
    int Work[numResources];
    for (int j = 0; j < numResources; j++) {
        Work[j] = Available[j];
    }

    // 'Finish' vector, a boolean array.
    // Use VLA since numProcesses is known.
    bool Finish[numProcesses];
    for (int i = 0; i < numProcesses; i++) {
        Finish[i] = false; // No process has finished yet.
    }

    int safeSeqIndex = 0; // Index for building the safeSequence array.

    // --- Step 2: Find a process that can finish ---
    int completedCount = 0;
    while (completedCount < numProcesses) {
        bool foundProcess = false; 

        // Iterate through all processes
        for (int p = 0; p < numProcesses; p++) {
            // Check if process P[p] has *not* finished yet
            if (Finish[p] == false) {
                
                // Check if Need[p] <= Work
                bool canRun = true;
                for (int j = 0; j < numResources; j++) {
                    if (Need[p][j] > Work[j]) {
                        canRun = false; // Cannot meet the need
                        break;
                    }
                }

                // --- Step 3: "Run" the process ---
                if (canRun) {
                    // Add its resources back to the 'Work' pool
                    for (int j = 0; j < numResources; j++) {
                        Work[j] += Allocation[p][j];
                    }

                    // Mark as finished
                    Finish[p] = true;

                    // Add to safe sequence
                    safeSequence[safeSeqIndex++] = p;
                    
                    foundProcess = true;
                    completedCount++;
                }
            }
        } // end of for(p...) loop

        // If no process could be found to run in a full pass,
        // the system is in an UNSAFE state.
        if (foundProcess == false) {
            return false; // No safe sequence exists
        }
    }

    // --- Step 4: Final Check ---
    // If we exit the loop, all processes are finished. System is SAFE.
    return true;
}

/**
 * @brief Implements the Resource-Request Algorithm.
 * Checks if a request from a process can be safely granted.
 * @param processID The ID of the process making the request (e.g., 0 for P0).
 * @param request   A vector (VLA) with the requested instances.
 * @return true if the request was granted, false if it was denied/made to wait.
 */
bool resourceRequest(int processID, int request[]) {
    // --- Step 1: Check if Request <= Need ---
    for (int j = 0; j < numResources; j++) {
        if (request[j] > Need[processID][j]) {
            printf("DENIED: Process P%d request exceeds its 'Need' matrix value.\n", processID);
            return false;
        }
    }

    // --- Step 2: Check if Request <= Available ---
    for (int j = 0; j < numResources; j++) {
        if (request[j] > Available[j]) {
            printf("DENIED: Process P%d must wait. Resources not available.\n", processID);
            return false;
        }
    }

    // --- Step 3: "Pretend" to allocate the resources ---
    // We will modify the *actual* state, but we save the old
    // values in VLAs so we can roll back if the state is unsafe.
    int oldAvailable[numResources];
    int oldAllocation[numResources];
    int oldNeed[numResources];

    for (int j = 0; j < numResources; j++) {
        // Save current state
        oldAvailable[j] = Available[j];
        oldAllocation[j] = Allocation[processID][j];
        oldNeed[j] = Need[processID][j];

        // Apply the hypothetical allocation
        Available[j] -= request[j];
        Allocation[processID][j] += request[j];
        Need[processID][j] -= request[j];
    }

    // --- Step 4: Run the Safety Algorithm on the hypothetical state ---
    printf("...running safety check on this hypothetical allocation...\n");
    
    // VLA for the temporary sequence
    int tempSafeSequence[numProcesses]; 
    
    if (isSafe(tempSafeSequence)) {
        // The new state is SAFE. Keep it.
        printf("GRANTED: Request by P%d is safe. Resources allocated.\n", processID);
        return true;
    } else {
        // The new state is UNSAFE. ROLL BACK.
        printf("DENIED: Granting request by P%d would lead to an UNSAFE state. Rolling back.\n", processID);

        // Roll back: Restore the saved state
        for (int j = 0; j < numResources; j++) {
            Available[j] = oldAvailable[j];
            Allocation[processID][j] = oldAllocation[j];
            Need[processID][j] = oldNeed[j];
        }
        return false;
    }
}

/**
 * @brief A utility function to print the current state of the system
 * (Allocation, Max, Need, and Available).
 */
void printState() {
    printf("Current System Snapshot:\n");
    
    // Print header
    printf("       %-*s   %-*s   %-*s\n",
           numResources * 2 + 1, "Allocation",
           numResources * 2 + 1, "Max",
           numResources * 2 + 1, "Need");

    // Print resource labels
    printf("       ");
    for (int k = 0; k < 3; k++) { // For each of the 3 matrices
        for (int j = 0; j < numResources; j++) {
            printf("R%d ", j);
        }
        printf("  ");
    }
    printf("\n");

    // Print matrix contents
    for (int i = 0; i < numProcesses; i++) {
        printf("P%-*d", 2, i); // "P0  "
        
        // Allocation
        for (int j = 0; j < numResources; j++) {
            printf("%-2d", Allocation[i][j]);
        }
        printf("   ");
        
        // Max
        for (int j = 0; j < numResources; j++) {
            printf("%-2d", Max[i][j]);
        }
        printf("   ");
        
        // Need
        for (int j = 0; j < numResources; j++) {
            printf("%-2d", Need[i][j]);
        }
        printf("\n");
    }
    
    // Print Available
    printf("\nAvailable: [ ");
    for (int j = 0; j < numResources; j++) {
        printf("%d ", Available[j]);
    }
    printf("]\n");
}