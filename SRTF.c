#include <stdio.h>

struct Process {
    int pid;
    int at;  // Arrival Time
    int bt;  // Burst Time
    int wt;  // Waiting Time
    int tat; // Turnaround Time
    int ct;  // Completion Time
    int rt;  // Remaining Time
};

int main() {
    int n;
    float total_wt = 0;
    float total_tat = 0;

    printf("Enter number of Processes:\n");
    scanf("%d", &n);

    // Create array of structure type process of size n
    struct Process p[n];

    // Take input for A.T. and B.T.
    for (int i = 0; i < n; i++) {
        p[i].pid = i;
        printf("Enter A.T. for Process with pid:%d: ", i);
        scanf("%d", &p[i].at);
        printf("Enter B.T. for Process with pid:%d: ", i);
        scanf("%d", &p[i].bt);
        p[i].rt = p[i].bt; // Initialize Remaining Time
    }
    printf("\n");

    int current_time = 0;
    int completed = 0;
    int shortest_index = -1;
    int shortest_rt = 99999;
    int found = 0; // Flag to check if a process was found

    // This is the main simulation loop (our "system clock")
    while (completed < n) {
        
        // --- 1. Find the best process to run ---
        // We must search all processes at every time unit
        shortest_rt = 99999; // Reset search
        shortest_index = -1;
        found = 0;

        for (int i = 0; i < n; i++) {
            // Check if process has arrived AND still needs to run
            if (p[i].at <= current_time && p[i].rt > 0) {
                // If it's shorter than the shortest one we've found
                if (p[i].rt < shortest_rt) {
                    shortest_rt = p[i].rt;
                    shortest_index = i;
                    found = 1;
                }
            }
        }

        // --- 2. Decide what to do based on the search ---

        // Case A: No process is ready (CPU is idle)
        if (found == 0) {
            current_time++; // Just advance the clock
        } 
        // Case B: A process was found, so "run" it
        else {
            // Decrement the remaining time
            p[shortest_index].rt--;

            // Increment the clock
            current_time++;

            // --- 3. Check if the process finished ---
            if (p[shortest_index].rt == 0) {
                // It's done! Increment completed count
                completed++;

                // Calculate its final stats
                p[shortest_index].ct = current_time;
                p[shortest_index].tat = p[shortest_index].ct - p[shortest_index].at;
                p[shortest_index].wt = p[shortest_index].tat - p[shortest_index].bt;

                // Add to totals
                total_wt += p[shortest_index].wt;
                total_tat += p[shortest_index].tat;
            }
        }
    } // End of while loop

    // --- 4. Print the final results ---
    printf("\n--- SRTF Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Waiting Time: %.2f\n", (total_wt / n));
    printf("Average Turnaround Time: %.2f\n", (total_tat / n));

    return 0;
}