# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = mysh
TEST_TARGET = test

# Default target
all: $(TARGET) $(TEST_TARGET) clean

# Build the main shell executable
$(TARGET): mysh.o custom_string.o memory.o command.o job.o custom_signals.o
	$(CC) $(CFLAGS) -o $(TARGET) mysh.o custom_string.o memory.o command.o job.o custom_signals.o

# Build the test executable
$(TEST_TARGET): test_driver.o custom_string.o memory.o command.o job.o custom_signals.o
	$(CC) $(CFLAGS) -o $(TEST_TARGET) test_driver.o custom_string.o memory.o command.o job.o custom_signals.o

# Object file rules
mysh.o: mysh.c 
	$(CC) $(CFLAGS) -c mysh.c

custom_string.o: custom_string.c custom_string.h
	$(CC) $(CFLAGS) -c custom_string.c

memory.o: memory.c memory.h custom_string.h
	$(CC) $(CFLAGS) -c memory.c

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c

job.o: job.c job.h
	$(CC) $(CFLAGS) -c job.c

custom_signals.o: custom_signals.c custom_signals.h
	$(CC) $(CFLAGS) -c custom_signals.c

test_driver.o: test_driver.c custom_string.h command.h memory.h job.h
	$(CC) $(CFLAGS) -c test_driver.c

# Clean up object files
clean:
	rm -f *.o *.txt