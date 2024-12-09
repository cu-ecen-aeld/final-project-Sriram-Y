# Cross-compilation setup
CROSS_COMPILE ?=
CC = $(CROSS_COMPILE)gcc

# Compiler flags
CFLAGS = -g -Wall -Wextra -O2

# Linker flags - ensure all required libraries are specified here
LDFLAGS = $(shell pkg-config --libs alsa) -lm -pthread -lasound

# Target binary
TARGET = audioeqdriver

# Source files
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the target binary
$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

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

