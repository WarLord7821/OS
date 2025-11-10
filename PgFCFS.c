#include <stdio.h>

// Function to print the frames
void printFrames(int frames[], int n) {
    for (int i = 0; i < n; i++) {
        if (frames[i] == -1) {
            printf("[_] "); // Print empty slot
        } else {
            printf("[%d] ", frames[i]);
        }
    }
    printf("\n");
}

int main() {
    int frame_count;
    int ref_len;
    
    printf("Enter number of page frames: ");
    scanf("%d", &frame_count);

    printf("Enter length of reference string: ");
    scanf("%d", &ref_len);

    int ref_string[ref_len];
    printf("Enter the reference string (e.g., 1 2 3 4...): ");
    for (int i = 0; i < ref_len; i++) {
        scanf("%d", &ref_string[i]);
    }

    int frames[frame_count];
    // Initialize all frames to -1 (empty)
    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
    }

    int page_faults = 0;
    int victim_frame = 0; // This is our FIFO "pointer"

    printf("\n--- FIFO Page Replacement Simulation ---\n");
    // Loop through each page in the reference string
    for (int i = 0; i < ref_len; i++) {
        int current_page = ref_string[i];
        int found = 0;

        // 1. Search if page is already in a frame
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == current_page) {
                found = 1;
                break; // Hit! No fault.
            }
        }

        // 2. If 'found' is still 0, it's a Page Fault
        if (found == 0) {
            page_faults++;
            
            // Place the page in the current "victim" frame
            frames[victim_frame] = current_page;
            
            // Update the victim_frame pointer for the *next* fault
            // This is the core FIFO logic
            victim_frame = (victim_frame + 1) % frame_count;
            
            printf("Fault (Page %d): ", current_page);
            printFrames(frames, frame_count);
        } else {
            printf("Hit   (Page %d): ", current_page);
            printFrames(frames, frame_count);
        }
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    return 0;
}