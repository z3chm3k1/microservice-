#include <stdio.h>
#include "Budget.h"
#include "Ui.h"
#include "budget_client.h"

int main() {
    float income = 0.0, expenses = 0.0;
    float pastIncomes[MAX_HISTORY] = {0};
    float pastExpenses[MAX_HISTORY] = {0};
    int incomeCount = 0, expenseCount = 0;
    int choice;
    char input[100];
    float new_limit;

    while (1) {
        display_ui();
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            
            printf("Invalid input. Please enter a number.\n");
            while ((getchar()) != '\n'); 
            continue;
        }

        while ((getchar()) != '\n'); 

        switch (choice) {
            case 1:
                add_income(&income, pastIncomes, &incomeCount);
                break;
            case 2:
                add_expense(&expenses, pastExpenses, &expenseCount);
                break;
            case 3:
                view_balance(income, expenses);
                break;
            case 4:
                view_past_income(pastIncomes, incomeCount);
                break;
            case 5:
                view_past_expenses(pastExpenses, expenseCount);
                break;
            case 6:
                delete_income_entry(pastIncomes, &incomeCount, &income);
                break;
            case 7:
                delete_expense_entry(pastExpenses, &expenseCount, &expenses);
                break;
            case 8:
                printf("\nEnter new budget limit: $");
                if (scanf("%s", input) != 1) {
                    printf("Invalid input.\n");
                    break;
                }
                new_limit = atof(input);
                if (new_limit <= 0) {
                    printf("Budget limit must be greater than zero.\n");
                    break;
                }
                set_budget_limit(new_limit);
                printf("Budget limit set to $%.2f\n", new_limit);
                break;
            case 9:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
