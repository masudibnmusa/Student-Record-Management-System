# Makefile for Student Record Management System

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = student_management
SRC = student_management.c

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Clean build files
clean:
	rm -f $(TARGET) $(TARGET).exe *.o

# Install (copy to /usr/local/bin on Unix-like systems)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean install uninstall run debug
