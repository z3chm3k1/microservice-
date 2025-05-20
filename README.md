# Budget Limit Microservice for Cam

To request data:
- Run both the main budget app and microservice simultaneously
- Add income/expenses through the budget app menu
- Set budget limits through the menu option

Alternatively:
- Write your request into `data/limit_request.json` Ex.:
  ```
  {"action": "check_limit", "timestamp": "2025-05-19T22:31:53"}
  ```

To receive the data:
- Use the "View Balance" option in the budget app
- Or read the response directly from `data/limit_response.json` Ex.:
  ```
  {"status": "warning", "percentage": 75.0, "message": "CAUTION: You are at 75.0% of your budget limit of $5000.00"}
  ```

Communication Contract:
| File                  | Role   | Description                                |
|-----------------------|--------|--------------------------------------------|
| limit_request.json    | Input  | Request for budget check                    |
| transactions.json     | Input  | Record of all income/expenses              |
| limit_config.json     | Input  | Budget limit configuration                 |
| limit_response.json   | Output | Budget status and warnings                 |

Setup:
1. Compile the microservice: `gcc -o budget_limit_service main.c budget_limit_service.c json_util.c -I.`
2. Run in separate terminal: `./budget_limit_service`
