# Get CROSS_COMPILE value from user
CROSS_COMPILE ?= 

# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -Wall -Werror

# Target binary
TARGET = audioeqdriver

# Source files and object files
SRC = $(TARGET).c
OBJ = $(SRC:.c=.o)

# Default
all: $(TARGET)
default: $(TARGET)

# Build executable from object files
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -pthread -lasound -lmp3lame -lfftw3 -o $@ $^

# Clean
clean:
	rm -rf *.o $(TARGET) *.elf *.map ./build && mkdir build

# Phony targets
.PHONY: 
	all clean
