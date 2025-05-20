#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "budget_client.h"

void add_income(float *income, float pastIncomes[], int *incomeCount) {
    char input[100];
    float amount;

    printf("\n**Enter 'b' to go back**\n");
    printf("Enter income amount: $");
    scanf("%s", input);

    if (strcmp(input, "b") == 0) {
        printf("Returning to main menu...\n");
        return;
    }

    amount = atof(input);
    if (amount > 0) {
        *income += amount;
        pastIncomes[*incomeCount] = amount;
        (*incomeCount)++;
        
        // Log the transaction to the microservice
        log_transaction("income", amount);
        
        printf("Income added successfully.\n");
    } else {
        printf("Invalid income amount.\n");
    }
}

void add_expense(float *expenses, float pastExpenses[], int *expenseCount) {
    char input[100];
    float amount;

    printf("\n**Enter 'b' to go back**\n");
    printf("Enter expense amount: $");
    scanf("%s", input);

    if (strcmp(input, "b") == 0) {
        printf("Returning to main menu...\n");
        return;
    }

    amount = atof(input);
    if (amount > 0) {
        *expenses += amount;
        pastExpenses[*expenseCount] = amount;
        (*expenseCount)++;
        
        // Log the transaction to the microservice
        log_transaction("expense", amount);
        
        // Request a budget check after adding an expense
        request_budget_check();
        
        // Get the response from the microservice
        BudgetLimitResponse response = read_budget_response();
        
        // Display warning if needed
        if (strcmp(response.status, "ok") != 0) {
            printf("\n%s\n", response.message);
        }
        
        printf("Expense added successfully.\n");
    } else {
        printf("Invalid expense amount.\n");
    }
}

void view_balance(float income, float expenses) {
    printf("\nCurrent Balance: $%.2f\n", income - expenses);
    
    // Request a budget check when viewing balance
    request_budget_check();
    
    // Get the response from the microservice
    BudgetLimitResponse response = read_budget_response();
    
    // Display budget status
    printf("Budget Status: %.1f%% of limit used\n", response.percentage);
    if (strcmp(response.status, "ok") != 0) {
        printf("%s\n", response.message);
    }
}

void view_past_income(float pastIncomes[], int incomeCount) {
    printf("\nIncome History:\n");
    if (incomeCount == 0) {
        printf("No income history available.\n");
    } else {
        for (int i = 0; i < incomeCount; i++) {
            printf("Income %d: $%.2f\n", i + 1, pastIncomes[i]);
        }
    }
}

void view_past_expenses(float pastExpenses[], int expenseCount) {
    printf("\nExpense History:\n");
    if (expenseCount == 0) {
        printf("No expense history available.\n");
    } else {
        for (int i = 0; i < expenseCount; i++) {
            printf("Expense %d: $%.2f\n", i + 1, pastExpenses[i]);
        }
    }
}

void delete_income_entry(float pastIncomes[], int *incomeCount, float *income) {
    if (*incomeCount == 0) {
        printf("No income entries to delete.\n");
        return;
    }

    char input[100];
    view_past_income(pastIncomes, *incomeCount);

    printf("\n**Enter 'b' to go back**\n");
    printf("Enter the income entry number to delete: ");
    scanf("%s", input);

    if (strcmp(input, "b") == 0) {
        printf("Returning to main menu...\n");
        return;
    }

    int index = atoi(input);
    if (index < 1 || index > *incomeCount) {
        printf("Invalid entry number.\n");
        return;
    }

    index--;

    // Ask for confirmation
    printf("Are you sure you want to permanently delete Income %d: $%.2f? (y/n): ", index + 1, pastIncomes[index]);
    scanf(" %s", input);  // Note the space before %s to consume any leftover newline
    if (input[0] != 'y' && input[0] != 'Y') {
        printf("Deletion canceled.\n");
        return;
    }

    *income -= pastIncomes[index];
    for (int i = index; i < *incomeCount - 1; i++) {
        pastIncomes[i] = pastIncomes[i + 1];
    }

    (*incomeCount)--;
    
    // Log the negative transaction to reflect deletion
    log_transaction("income", -pastIncomes[index]);
    
    printf("Income entry deleted.\n");
}

void delete_expense_entry(float pastExpenses[], int *expenseCount, float *expenses) {
    if (*expenseCount == 0) {
        printf("No expense entries to delete.\n");
        return;
    }

    char input[100];
    view_past_expenses(pastExpenses, *expenseCount);

    printf("\n**Enter 'b' to go back**\n");
    printf("Enter the expense entry number to delete: ");
    scanf("%s", input);

    if (strcmp(input, "b") == 0) {
        printf("Returning to main menu...\n");
        return;
    }

    int index = atoi(input);
    if (index < 1 || index > *expenseCount) {
        printf("Invalid entry number.\n");
        return;
    }

    index--;

    // Ask for confirmation
    printf("Are you sure you want to permanently delete Expense %d: $%.2f? (y/n): ", index + 1, pastExpenses[index]);
    scanf(" %s", input);  // Note the space before %s to consume any leftover newline
    if (input[0] != 'y' && input[0] != 'Y') {
        printf("Deletion canceled.\n");
        return;
    }

    *expenses -= pastExpenses[index];
    for (int i = index; i < *expenseCount - 1; i++) {
        pastExpenses[i] = pastExpenses[i + 1];
    }

    (*expenseCount)--;
    
    // Log the negative transaction to reflect deletion
    log_transaction("expense", -pastExpenses[index]);
    
    // Request a budget check after deleting an expense
    request_budget_check();
    
    // Get the response from the microservice
    BudgetLimitResponse response = read_budget_response();
    
    // Display updated status
    if (strcmp(response.status, "ok") != 0) {
        printf("\n%s\n", response.message);
    }
    
    printf("Expense entry deleted.\n");
}
