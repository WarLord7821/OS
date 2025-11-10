#include <stdio.h>

// Helper function to print the frames
void printFrames(int frames[], int n) {
    for (int i = 0; i < n; i++) {
        if (frames[i] == -1) {
            printf("[_] ");
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
    printf("Enter the reference string: ");
    for (int i = 0; i < ref_len; i++) {
        scanf("%d", &ref_string[i]);
    }

    int frames[frame_count];
    // This array stores the "time" of the last access for each frame
    int last_used_time[frame_count]; 

    // Initialize all frames to -1 (empty) and time to 0
    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
        last_used_time[i] = 0;
    }

    int page_faults = 0;
    int time_counter = 0; // This acts as our global "clock"

    printf("\n--- LRU Page Replacement Simulation ---\n");
    
    // Loop through each page in the reference string
    for (int i = 0; i < ref_len; i++) {
        int current_page = ref_string[i];
        int found = 0;

        time_counter++; // Increment our clock for each page reference

        // 1. Search if page is already in a frame
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == current_page) {
                found = 1;
                // It's a HIT. Update its last used time to "now".
                last_used_time[j] = time_counter; 
                break;
            }
        }

        // 2. If 'found' is still 0, it's a Page Fault
        if (found == 0) {
            page_faults++;

            // 2a. First, check if there's any empty frame
            int empty_frame_index = -1;
            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    empty_frame_index = j;
                    break;
                }
            }

            if (empty_frame_index != -1) {
                // We found an empty frame, use it.
                frames[empty_frame_index] = current_page;
                last_used_time[empty_frame_index] = time_counter;
            } else {
                // 2b. No empty frames. Must find the LRU victim.
                // Find the frame with the "minimum" (oldest) last_used_time.
                int lru_index = 0;
                int min_time = last_used_time[0];

                for (int j = 1; j < frame_count; j++) {
                    if (last_used_time[j] < min_time) {
                        min_time = last_used_time[j];
                        lru_index = j;
                    }
                }
                
                // Replace the LRU victim
                frames[lru_index] = current_page;
                last_used_time[lru_index] = time_counter;
            }
            
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