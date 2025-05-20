#include "budget_client.h"

// Simple JSON string value extraction
// Format: "key": "value" or "key": value
static char* json_get_string_value(const char* json, const char* key) {
    char search_key[256];
    sprintf(search_key, "\"%s\":", key);
    
    char* key_pos = strstr(json, search_key);
    if (key_pos == NULL) {
        return NULL;
    }
    
    // Move past the key
    key_pos += strlen(search_key);
    
    // Skip whitespace
    while (*key_pos == ' ' || *key_pos == '\t') {
        key_pos++;
    }
    
    // Check if it's a string value (starts with ")
    if (*key_pos == '"') {
        key_pos++; // Skip the opening quote
        
        // Find the closing quote
        char* end_pos = strchr(key_pos, '"');
        if (end_pos == NULL) {
            return NULL;
        }
        
        // Calculate the length of the value
        size_t value_len = end_pos - key_pos;
        
        // Allocate memory for the value
        char* value = (char*)malloc(value_len + 1);
        if (value == NULL) {
            return NULL;
        }
        
        // Copy the value
        strncpy(value, key_pos, value_len);
        value[value_len] = '\0';
        
        return value;
    }
    
    return NULL;
}

// Extract a float value from a JSON string
static float json_get_float_value(const char* json, const char* key) {
    char search_key[256];
    sprintf(search_key, "\"%s\":", key);
    
    char* key_pos = strstr(json, search_key);
    if (key_pos == NULL) {
        return 0.0f;
    }
    
    // Move past the key
    key_pos += strlen(search_key);
    
    // Skip whitespace
    while (*key_pos == ' ' || *key_pos == '\t') {
        key_pos++;
    }
    
    // Parse the float value
    return atof(key_pos);
}

// Function to get the current timestamp as a string
static char* get_timestamp() {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    static char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", tm_info);
    
    return timestamp;
}

// Function to log a transaction (income or expense)
void log_transaction(const char* type, float amount) {
    FILE* transaction_file = fopen(TRANSACTION_FILE, "a");
    if (transaction_file == NULL) {
        // Create the data directory if it doesn't exist
        system("mkdir -p data");
        
        transaction_file = fopen(TRANSACTION_FILE, "a");
        if (transaction_file == NULL) {
            return;
        }
    }
    
    // Write the transaction as a JSON object
    fprintf(transaction_file, "{\"timestamp\": \"%s\", \"type\": \"%s\", \"amount\": %.2f}\n", 
            get_timestamp(), type, amount);
    
    fclose(transaction_file);
}

// Function to request a budget limit check
void request_budget_check() {
    FILE* request_file = fopen(REQUEST_FILE, "w");
    if (request_file == NULL) {
        // Create the data directory if it doesn't exist
        system("mkdir -p data");
        
        request_file = fopen(REQUEST_FILE, "w");
        if (request_file == NULL) {
            return;
        }
    }
    
    // Write the request as a JSON object
    fprintf(request_file, "{\"action\": \"check_limit\", \"timestamp\": \"%s\"}\n", get_timestamp());
    
    fclose(request_file);
}

// Function to read the entire contents of a file
static char* read_file(const char* filename) {
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

// Function to read the budget limit response
BudgetLimitResponse read_budget_response() {
    BudgetLimitResponse response = {"unknown", 0.0, "No response available"};
    
    char* response_data = read_file(RESPONSE_FILE);
    if (response_data == NULL) {
        return response;
    }
    
    // Parse the response
    char* status = json_get_string_value(response_data, "status");
    if (status != NULL) {
        strncpy(response.status, status, sizeof(response.status) - 1);
        free(status);
    }
    
    response.percentage = json_get_float_value(response_data, "percentage");
    
    char* message = json_get_string_value(response_data, "message");
    if (message != NULL) {
        strncpy(response.message, message, sizeof(response.message) - 1);
        free(message);
    }
    
    free(response_data);
    return response;
}

// Function to set a new budget limit
void set_budget_limit(float limit) {
    if (limit <= 0) {
        return;
    }
    
    FILE* config_file = fopen(CONFIG_FILE, "w");
    if (config_file == NULL) {
        // Create the data directory if it doesn't exist
        system("mkdir -p data");
        
        config_file = fopen(CONFIG_FILE, "w");
        if (config_file == NULL) {
            return;
        }
    }
    
    // Write the config as a JSON object
    fprintf(config_file, "{\"monthly_limit\": %.2f, \"warning_threshold\": 80.0}", limit);
    
    fclose(config_file);
}
