# Compiler and flags
CC = arm-none-linux-gnueabihf-gcc
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
	$(CC) $(CFLAGS) -pthread -lasound -o $@ $^

# Clean
clean:
	rm -f *.o $(TARGET) *.elf *.map

# Phony targets
.PHONY: 
	all clean
