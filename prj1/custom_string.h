#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>



int string_compare(const char * string1, const char* string2, size_t byte_length);
int parse (char *input_buffer, char *args[]);
char *strcpy (char *destination, const char *source); 

size_t get_strlen(const char *input_str);

char *strncat(char *destination, const char *source, size_t src_byte_len);
int contains_pipe(char* argv[]);

// char *strcat(char *destination, const char *source);  

#endif
