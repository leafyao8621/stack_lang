AS = as
LD = ld
SRC = main.s
OBJ = main.o
BIN = main

%.o: %.s
	$(AS) $< -o $@

$(BIN): $(OBJ)
	$(LD) $(OBJ) -o $(BIN)

.PHONY: clean
clean:
	@rm $(OBJ)
	@rm $(BIN)
