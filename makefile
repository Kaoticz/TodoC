# Makefile for the project

CC = gcc
CFLAGS = -fdiagnostics-color=always -Wall -Wextra -Winline -Wunreachable-code -Wmain -pedantic -lsqlite3 -g
SRC_DIR = ./
UTILITIES_DIR = utilities
BIN_DIR = bin

# Source files
ifeq ($(OS),Windows_NT)
    SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.h)
else
    SRCS = $(shell find $(SRC_DIR) -name '*.c' -o -name '*.h')
endif

# Object files in the bin/ directory
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

# The target executable
TARGET = $(BIN_DIR)/main

all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR)/