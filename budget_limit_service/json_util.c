#include "json_util.h"

// Extract a string value from a JSON string
char* json_get_string_value(const char* json, const char* key) {
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
float json_get_float_value(const char* json, const char* key) {
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

// Create a simple JSON response
char* json_create_response(const char* status, float percentage, const char* message) {
    char* response = (char*)malloc(512); // Allocate enough space for the response
    if (response == NULL) {
        return NULL;
    }
    
    sprintf(response, "{\"status\": \"%s\", \"percentage\": %.1f, \"message\": \"%s\"}", 
            status, percentage, message);
    
    return response;
}
