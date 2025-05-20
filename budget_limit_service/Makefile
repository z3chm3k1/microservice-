CC = gcc
CFLAGS = -Wall -g
OBJ = main.o budget_limit_service.o json_util.o

all: budget_limit_service

budget_limit_service: $(OBJ)
	$(CC) $(CFLAGS) -o budget_limit_service $(OBJ)

main.o: main.c budget_limit_service.h
budget_limit_service.o: budget_limit_service.c budget_limit_service.h json_util.h
json_util.o: json_util.c json_util.h

clean:
	rm -f *.o budget_limit_service
