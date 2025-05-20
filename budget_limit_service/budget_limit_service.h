#ifndef BUDGET_LIMIT_SERVICE_H
#define BUDGET_LIMIT_SERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRANSACTION_FILE "../data/transactions.json"
#define REQUEST_FILE "../data/limit_request.json"
#define RESPONSE_FILE "../data/limit_response.json"
#define CONFIG_FILE "../data/limit_config.json"

// Default budget limit if no config file exists
#define DEFAULT_LIMIT 1000.0f
#define DEFAULT_WARNING_THRESHOLD 80.0f

// Function to read the current budget limit from config file
float read_budget_limit();

// Function to calculate total expenses from transaction file
float calculate_total_expenses();

// Function to check if expenses are approaching the limit
void check_budget_limit();

// Function to process incoming requests
void process_request(const char* request);

// Function to write a response
void write_response(const char* status, float percentage, const char* message);

#endif
