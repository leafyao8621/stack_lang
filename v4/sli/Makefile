CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/parser/*.c) $(wildcard src/parser/*/*.c) $(wildcard src/parser/*/*/*.c) $(wildcard src/util/*.c) $(wildcard src/interpreter/*/*.c) $(wildcard src/interpreter/*/*/*.c) $(wildcard src/interpreter/*/*/*/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/libsliv4.so
BIN = sli

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	$(CC) $(OBJ) -o $(LIB) -lcontainers -shared

$(BIN): $(LIB)
	$(CC) -g src/interpreter/main.c -o $(BIN) -lcontainers -Llib -lsliv4  -Iinclude -Wl,-rpath,lib

.PHONY: clean all
all: $(BIN)
	@echo "compiled"
valgrind: $(BIN)
	@valgrind -s --leak-check=full ./$(BIN)
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
install: $(BIN)
	@cp $(BIN) ~/.local/bin
	@cp -r include/sliv4 ~/.local/include
	@cp $(LIB) ~/.local/lib
