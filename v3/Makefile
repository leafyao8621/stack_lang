CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
SRC_INTERPRETER = $(wildcard src/parser/*.c) $(wildcard src/interpreter/*/*.c) $(wildcard src/util/*.c)
OBJ_INTERPRETER = $(SRC_INTERPRETER:.c=.o)
LIB_INTERPRETER = lib/libsliv3.so
BIN_INTERPRETER = sli

%.o: %.c
	@cp src/util/errcode.h src/interpreter/core/core.h src/parser/parser.h include/sliv3
	$(CC) $(CFLAGS) -g -c $< -o $@ -Iinclude

$(LIB_INTERPRETER): $(OBJ_INTERPRETER)
	$(CC) $(OBJ_INTERPRETER) -o $(LIB_INTERPRETER) -lcontainers -shared

$(BIN_INTERPRETER): $(LIB_INTERPRETER)
	$(CC) src/interpreter/main.c -o $(BIN_INTERPRETER) -lcontainers -Llib -lsliv3  -Iinclude -Wl,-rpath,lib

.PHONY: clean all
all: $(BIN_INTERPRETER)
	@echo "compiled"
clean:
	@rm $(OBJ_INTERPRETER)
	@rm $(LIB_INTERPRETER)
	@rm $(BIN_INTERPRETER)
install: $(BIN_INTERPRETER)
	@cp $(BIN_INTERPRETER) ~/.local/bin
	@cp -r include/sliv3 ~/.local/include
	@cp $(LIB) ~/.local/lib
