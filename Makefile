CC=gcc
CFLAGS=-Wall -g
OBJ=main.o Budget.o Ui.o budget_client.o

all: budget_app budget_limit_service

budget_app: $(OBJ)
	$(CC) -o budget_app $(OBJ)

budget_limit_service:
	cd budget_limit_service && $(MAKE)

main.o: main.c Budget.h Ui.h budget_client.h
Budget.o: Budget.c Budget.h budget_client.h
Ui.o: Ui.c Ui.h
budget_client.o: budget_client.c budget_client.h

clean:
	rm -f *.o budget_app
	cd budget_limit_service && $(MAKE) clean
