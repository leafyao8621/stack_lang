CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/parser/*.c) $(wildcard src/parser/*/*.c) $(wildcard src/parser/*/*/*.c) $(wildcard src/util/*.c) $(wildcard src/interpreter/*/*.c) $(wildcard src/interpreter/*/*/*.c) $(wildcard src/interpreter/*/*/*/*.c)
OBJ = $(SRC:.c=.o)
LIB = lib/libsliv3.so
BIN = sli

%.o: %.c
	@cp src/util/errcode.h src/interpreter/core/core.h src/parser/parser.h include/sliv3
	$(CC) $(CFLAGS) -g -O3 -c $< -o $@ -Iinclude

$(LIB): $(OBJ)
	$(CC) $(OBJ) -o $(LIB) -lcontainers -shared

$(BIN): $(LIB)
	$(CC) src/interpreter/main.c -o $(BIN) -lcontainers -Llib -lsliv3  -Iinclude -Wl,-rpath,lib

.PHONY: clean all
all: $(BIN)
	@echo "compiled"
valgrind: $(BIN)
	@valgrind -s --leak-check=full ./$(BIN) ../../programs/v3/e.sl
clean:
	@rm $(OBJ)
	@rm $(LIB)
	@rm $(BIN)
install: $(BIN)
	@cp $(BIN) ~/.local/bin
	@cp -r include/sliv3 ~/.local/include
	@cp $(LIB) ~/.local/lib
