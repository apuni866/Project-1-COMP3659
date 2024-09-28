#ifndef MEMORY_H
#define MEMORY_H


#define HEAP_SIZE 1024

char *alloc(unsigned int size);
char *resize(char *old_buffer);
void free_all();


#endif
