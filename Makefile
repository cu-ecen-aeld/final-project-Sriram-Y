# Get CROSS_COMPILE value from user
CROSS_COMPILE ?= 

# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -Wall 
LDFLAGS = -lasound -lm -ldl -pthread -lmp3lame

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
	@if [ -f final_output.mp3 ]; then rm final_output.mp3; fi

# Phony targets
.PHONY: 
	all clean
