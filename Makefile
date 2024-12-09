# Get CROSS_COMPILE value from user
CROSS_COMPILE ?= 

# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS += -g -Wall $(shell pkg-config --cflags alsa)
LDFLAGS += $(shell pkg-config --libs alsa) -lfftw3 -lm -pthread -lasound

# Target binary
TARGET = audioeqdriver

# Source files and object files
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# Default
all: $(TARGET)
# default: $(TARGET)

# Build executable from object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Clean
clean:
	rm -f *.o $(TARGET) *.elf *.map

