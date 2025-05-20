# Budget Limit Microservice: Integration Guide

## How to Interact with the Budget Limit Microservice

### To REQUEST Data from the Microservice:

Run both the main budget application and the microservice program simultaneously. Then:

1. Use the budget application's menu to add income, add expenses, and set budget limits
2. When you add an expense or set a budget limit, the application will automatically send a request to the microservice

Alternatively, you can manually create the necessary files:

Create a JSON request file at `data/limit_request.json` with the following format:

```json
{"action": "check_limit", "timestamp": "CURRENT_TIMESTAMP"}
```

Where `CURRENT_TIMESTAMP` is the current time in ISO format (YYYY-MM-DDThh:mm:ss), for example: `2025-05-19T22:31:53`.

Log transactions (optional but recommended) by writing to `data/transactions.json`:

```json
{"timestamp": "CURRENT_TIMESTAMP", "type": "income", "amount": 1000.00}
{"timestamp": "CURRENT_TIMESTAMP", "type": "expense", "amount": 500.00}
```

Each transaction should be on a new line.

Set budget limit (optional) by writing to `data/limit_config.json`:

```json
{"monthly_limit": 5000.00, "warning_threshold": 80.0}
```

### To RECEIVE Data from the Microservice:

Use the budget application's "View Balance" option to see the budget status information. The application will automatically retrieve and display the current budget status, percentage used, and any warning messages.

Alternatively, you can directly check the response file at `data/limit_response.json`. The response will be in this format:

```json
{"status": "warning", "percentage": 75.0, "message": "CAUTION: You are at 75.0% of your budget limit of $5000.00"}
```

Extract the information you need:
- **status**: The current budget status (ok, notice, caution, warning, critical, exceeded)
- **percentage**: The percentage of budget used (0-100+)
- **message**: A human-readable message describing the budget status

The microservice must be running separately for this communication to work. It continuously monitors for changes to the request file and updates the response file accordingly."}
```

Extract the information you need:
- **status**: The current budget status (ok, notice, caution, warning, critical, exceeded)
- **percentage**: The percentage of budget used (0-100+)
- **message**: A human-readable message describing the budget status

The microservice must be running separately for this communication to work. It continuously monitors for changes to the request file and updates the response file accordingly.

## UML Sequence Diagram

[sequence_diagram.pdf](https://github.com/user-attachments/files/20327130/sequence_diagram.pdf)


## Status Codes

The microservice returns the following status codes:

- `ok`: < 50% of budget used
- `notice`: 50-74.9% of budget used
- `caution`: 75-89.9% of budget used
- `warning`: 90-94.9% of budget used
- `critical`: 95-99.9% of budget used
- `exceeded`: >= 100% of budget used

## Setup Instructions

1. Clone the repository
2. Compile the microservice:
   ```
   gcc -o budget_limit_service main.c budget_limit_service.c json_util.c -I.
   ```
3. Run the microservice in a separate terminal:
   ```
   ./budget_limit_service
   ```
4. Create the `data` directory in your main application directory if it doesn't exist
5. Ensure your main application can write to and read from the shared JSON files
