CC = gcc
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = sh

%.o: %.c
	$(CC) -g -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) -lncurses

.PHONY: clean
clean:
	@rm $(OBJ)
	@rm $(BIN)
