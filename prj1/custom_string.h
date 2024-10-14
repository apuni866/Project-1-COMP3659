#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "command.h"

int string_compare(const char *string1, const char *string2, size_t byte_length);

char *strcpy(char *destination, const char *source);

size_t get_strlen(const char *input_str);

char *strncat(char *destination, const char *source, size_t src_byte_len);
int contains_pipe_char(Command *command);
int contains_redirection_char(Command *command);

// char *strcat(char *destination, const char *source);

#endif
