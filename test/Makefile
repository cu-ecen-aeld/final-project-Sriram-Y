# Cross-compilation setup
CROSS_COMPILE ?= 
CC = $(CROSS_COMPILE)gcc

# Compiler flags
CFLAGS = -g -Wall -Wextra -O2

# Target binary
TARGET = main

# Source files
SRC = main.c
OBJ = $(SRC:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target binary
$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean
