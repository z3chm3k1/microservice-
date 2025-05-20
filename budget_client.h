#ifndef BUDGET_CLIENT_H
#define BUDGET_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// File paths for communication with the microservice
#define TRANSACTION_FILE "data/transactions.json"
#define REQUEST_FILE "data/limit_request.json"
#define RESPONSE_FILE "data/limit_response.json"
#define CONFIG_FILE "data/limit_config.json"

// Structure to hold budget limit response
typedef struct {
    char status[20];
    float percentage;
    char message[256];
} BudgetLimitResponse;

// Function to log a transaction (income or expense)
void log_transaction(const char* type, float amount);

// Function to request a budget limit check
void request_budget_check();

// Function to read the budget limit response
BudgetLimitResponse read_budget_response();

// Function to set a new budget limit
void set_budget_limit(float limit);

#endif
