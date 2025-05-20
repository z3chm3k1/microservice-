#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include "budget_limit_service.h"

#define POLL_INTERVAL_SEC 1

// Function to check if a file has been modified since last check
int file_modified(const char* filename, time_t* last_modified) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == 0) {
        if (file_stat.st_mtime > *last_modified) {
            *last_modified = file_stat.st_mtime;
            return 1;
        }
    }
    return 0;
}

// Function to read the entire contents of a file
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    // Allocate memory for the file content
    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Read the file content
    size_t bytes_read = fread(buffer, 1, file_size, file);
    buffer[bytes_read] = '\0';
    
    fclose(file);
    return buffer;
}

int main() {
    printf("Budget Limit Microservice - Starting...\n");
    printf("Monitoring for requests in %s\n", REQUEST_FILE);
    
    // Initialize last modified times
    time_t last_request_modified = 0;
    
    // Main service loop
    while (1) {
        // Check if request file has been modified
        if (file_modified(REQUEST_FILE, &last_request_modified)) {
            printf("Request file modified, processing...\n");
            
            // Read the request
            char* request = read_file(REQUEST_FILE);
            if (request != NULL) {
                // Process the request
                process_request(request);
                free(request);
            }
        }
        
        // Sleep for a short interval
        sleep(POLL_INTERVAL_SEC);
    }
    
    return 0;
}
