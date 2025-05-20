#include "budget_limit_service.h"
#include "json_util.h"

// Function to read the current budget limit from config file
float read_budget_limit() {
    FILE* config_file = fopen(CONFIG_FILE, "r");
    if (config_file == NULL) {
        // Create default config if it doesn't exist
        config_file = fopen(CONFIG_FILE, "w");
        if (config_file != NULL) {
            fprintf(config_file, "{\"monthly_limit\": %.2f, \"warning_threshold\": %.2f}", 
                    DEFAULT_LIMIT, DEFAULT_WARNING_THRESHOLD);
            fclose(config_file);
        }
        return DEFAULT_LIMIT;
    }
    
    // Read the file content
    char buffer[1024] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, config_file);
    fclose(config_file);
    
    if (bytes_read == 0) {
        return DEFAULT_LIMIT;
    }
    
    // Parse the limit from the JSON
    return json_get_float_value(buffer, "monthly_limit");
}

// Function to calculate total expenses from transaction file
float calculate_total_expenses() {
    FILE* transaction_file = fopen(TRANSACTION_FILE, "r");
    if (transaction_file == NULL) {
        return 0.0f;
    }
    
    float total_expenses = 0.0f;
    char line[1024];
    
    while (fgets(line, sizeof(line), transaction_file) != NULL) {
        // Check if this is an expense entry
        char* type = json_get_string_value(line, "type");
        if (type != NULL && strcmp(type, "expense") == 0) {
            float amount = json_get_float_value(line, "amount");
            total_expenses += amount;
            free(type);
        } else if (type != NULL) {
            free(type);
        }
    }
    
    fclose(transaction_file);
    return total_expenses;
}

// Function to check if expenses are approaching the limit
void check_budget_limit() {
    float limit = read_budget_limit();
    float total_expenses = calculate_total_expenses();
    float percentage = (limit > 0) ? (total_expenses / limit) * 100.0f : 0.0f;
    
    char message[256];
    const char* status;
    
    if (percentage >= 100.0f) {
        status = "exceeded";
        sprintf(message, "ALERT: You have EXCEEDED your budget limit of $%.2f", limit);
    } else if (percentage >= 95.0f) {
        status = "critical";
        sprintf(message, "URGENT: You are at %.1f%% of your budget limit of $%.2f", percentage, limit);
    } else if (percentage >= 90.0f) {
        status = "warning";
        sprintf(message, "WARNING: You are at %.1f%% of your budget limit of $%.2f", percentage, limit);
    } else if (percentage >= 75.0f) {
        status = "caution";
        sprintf(message, "CAUTION: You are at %.1f%% of your budget limit of $%.2f", percentage, limit);
    } else if (percentage >= 50.0f) {
        status = "notice";
        sprintf(message, "NOTICE: You are at %.1f%% of your budget limit of $%.2f", percentage, limit);
    } else {
        status = "ok";
        sprintf(message, "You are within budget limits (%.1f%% of $%.2f)", percentage, limit);
    }
    
    // Print to the microservice terminal with color-coded messages based on threshold
    printf("\n===== BUDGET STATUS UPDATE =====\n");
    printf("Current expenses: $%.2f\n", total_expenses);
    printf("Budget limit: $%.2f\n", limit);
    
    if (percentage >= 100.0f) {
        printf("\033[1;31mALERT: %.1f%% OF BUDGET USED - LIMIT EXCEEDED!\033[0m\n", percentage);
        printf("\033[1;31m%s\033[0m\n", message);
    } else if (percentage >= 95.0f) {
        printf("\033[31mURGENT: %.1f%% of budget used - Critical level reached\033[0m\n", percentage);
        printf("\033[31m%s\033[0m\n", message);
    } else if (percentage >= 90.0f) {
        printf("\033[33mWARNING: %.1f%% of budget used - Approaching limit\033[0m\n", percentage);
        printf("\033[33m%s\033[0m\n", message);
    } else if (percentage >= 75.0f) {
        printf("\033[36mCAUTION: %.1f%% of budget used - Monitor spending\033[0m\n", percentage);
        printf("\033[36m%s\033[0m\n", message);
    } else if (percentage >= 50.0f) {
        printf("\033[32mNOTICE: %.1f%% of budget used - Halfway point reached\033[0m\n", percentage);
        printf("\033[32m%s\033[0m\n", message);
    } else {
        printf("Status: %.1f%% of budget used - Within safe limits\n", percentage);
        printf("%s\n", message);
    }
    printf("==============================\n\n");
    
    write_response(status, percentage, message);
}

// Function to process incoming requests
void process_request(const char* request) {
    if (request == NULL || strlen(request) == 0) {
        return;
    }
    
    char* action = json_get_string_value(request, "action");
    if (action == NULL) {
        return;
    }
    
    if (strcmp(action, "check_limit") == 0) {
        check_budget_limit();
    }
    
    free(action);
}

// Function to write a response
void write_response(const char* status, float percentage, const char* message) {
    char* response = json_create_response(status, percentage, message);
    if (response == NULL) {
        return;
    }
    
    FILE* response_file = fopen(RESPONSE_FILE, "w");
    if (response_file != NULL) {
        fprintf(response_file, "%s", response);
        fclose(response_file);
    }
    
    free(response);
}
