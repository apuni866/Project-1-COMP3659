
# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = prompt


all: $(TARGET) clean

$(TARGET): prompt.o custom_string.o
	$(CC) $(CFLAGS) -o $(TARGET) prompt.o custom_string.o

prompt.o: prompt.c 
	$(CC) $(CFLAGS) -c prompt.c

custom_string.o: custom_string.c custom_string.h
	$(CC) $(CFLAGS) -c custom_string.c

clean:
	rm -f *.o
