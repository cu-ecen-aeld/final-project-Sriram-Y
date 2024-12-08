# Get CROSS_COMPILE value from user
CROSS_COMPILE ?= 

# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -Wall -pthread
LIBS = -lasound -lmp3lame -lm

# Target binary
TARGET = audioeqdriver

# Source files 
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

default: $(TARGET)

# Build target from object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf *.o $(TARGET)

# Phony targets
.PHONY: 
	all clean
