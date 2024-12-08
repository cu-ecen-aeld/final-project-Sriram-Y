# Get CROSS_COMPILE value from user
CROSS_COMPILE ?= 

# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -Wall 
LDFLAGS = -lasound -lmp3lame -lfftw3 -lm

# Target binary
TARGET = audioeqdriver

# Source files and object files
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Default
all: $(TARGET)
default: $(TARGET)

# Build executable from object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ $(LDFLAGS)

# Clean
clean:
	rm -f *.o $(TARGET) *.elf *.map

# Phony targets
.PHONY: 
	all clean
