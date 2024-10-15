# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = prompt
TEST_TARGET = test

# Default target
all: $(TARGET) $(TEST_TARGET) clean

# Build the main shell executable
$(TARGET): prompt.o custom_string.o memory.o command.o
	$(CC) $(CFLAGS) -o $(TARGET) prompt.o custom_string.o memory.o command.o

# Build the test executable
$(TEST_TARGET): test_driver.o custom_string.o memory.o command.o
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_driver.o custom_string.o memory.o command.o

# Object file rules
prompt.o: prompt.c 
	$(CC) $(CFLAGS) -c prompt.c

custom_string.o: custom_string.c custom_string.h
	$(CC) $(CFLAGS) -c custom_string.c

memory.o: memory.c memory.h custom_string.h
	$(CC) $(CFLAGS) -c memory.c

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c

test_driver.o: test_driver.c custom_string.h command.h memory.h
	$(CC) $(CFLAGS) -c test_driver.c

# Clean up object files
clean:
	rm -f *.o
