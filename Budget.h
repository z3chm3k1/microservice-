#ifndef BUDGET_H
#define BUDGET_H

void add_income(float *income, float pastIncomes[], int *incomeCount);
void add_expense(float *expenses, float pastExpenses[], int *expenseCount);
void view_balance(float income, float expenses);
void view_past_income(float pastIncomes[], int incomeCount);
void view_past_expenses(float pastExpenses[], int expenseCount);
void delete_income_entry(float pastIncomes[], int *incomeCount, float *income);
void delete_expense_entry(float pastExpenses[], int *expenseCount, float *expenses);


#endif