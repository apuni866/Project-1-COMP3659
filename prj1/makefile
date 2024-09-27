
# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = prompt


all: $(TARGET) clean

$(TARGET): prompt.o custom_string.o memory.o command.o
	$(CC) $(CFLAGS) -o $(TARGET) prompt.o custom_string.o memory.o command.o

prompt.o: prompt.c 
	$(CC) $(CFLAGS) -c prompt.c

custom_string.o: custom_string.c custom_string.h
	$(CC) $(CFLAGS) -c custom_string.c

memory.o: memory.c memory.h
	$(CC) $(CFLAGS) -c memory.c

command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c

clean:
	rm -f *.o
