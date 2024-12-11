# Get CROSS_COMPILE value from user
CROSS_COMPILE ?= 

# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -Wall 
LDFLAGS = -lasound -lmp3lame -lm -ldl -pthread

# Target binary
TARGET = audioeqdriver

# Source files and object files
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Default
all: $(TARGET)

# Build executable from object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean
clean:
	rm -f *.o $(TARGET) *.elf *.map

# Phony targets
.PHONY: 
	all clean
