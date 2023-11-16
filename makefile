# Makefile for the project

CC = gcc
CFLAGS = -fdiagnostics-color=always -Wall -Wextra -Winline -Wunreachable-code -Wmain -pedantic -lsqlite3 -g
SRC_DIR = .
UTILITIES_DIR = utilities
OBJ_DIR = obj
BIN_DIR = bin
EXEC_NAME = main

# Source files
ifeq ($(OS),Windows_NT)
    SRCS = $(wildcard $(SRC_DIR)/*.c)
else
    SRCS = $(shell find $(SRC_DIR)/ -name '*.c')
endif

# Object files in the obj/ directory
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# The target executable
TARGET = $(BIN_DIR)/$(EXEC_NAME)

all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR)/$(EXEC_NAME) $(OBJ_DIR)/