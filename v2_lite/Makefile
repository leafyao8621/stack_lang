CC = clang
CFLAGS = -std=c89 -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
SRC_COMPILER = $(wildcard src/parser/*.c) $(wildcard src/compiler/*.c) $(wildcard src/compiler/*/*.c) $(wildcard src/compiler/*/*/*/*.c) $(wildcard src/util/*.c) $(wildcard src/util/*/*.c)
OBJ_COMPILER = $(SRC_COMPILER:.c=.o)
BIN_COMPILER = slc

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@

$(BIN_COMPILER): $(OBJ_COMPILER)
	$(CC) $(OBJ_COMPILER) -o $(BIN_COMPILER)

.PHONY: clean all
all: $(BIN_COMPILER)
	@echo "compiled"
clean:
	@rm $(OBJ_COMPILER)
	@rm $(BIN_COMPILER)
install: $(BIN_COMPILER)
	@cp $(BIN_COMPILER) ~/.local/bin
