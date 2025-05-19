CCC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
SRC = src
TEST = tests
BUILD = build

FILES = $(SRC)/queuelist.c $(SRC)/stack.c $(SRC)/recursion.c $(SRC)/bst.c
TEST_FILE = $(TEST)/main.c
BIN = $(BUILD)/dictionary_manager

all: $(BIN)

$(BIN): $(FILES) $(TEST_FILE)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(FILES) $(TEST_FILE) -o $(BIN)

run: all
	./$(BIN)

clean:
	rm -rf $(BUILD)

.PHONY: all clean run

