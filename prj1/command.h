#ifndef COMMAND_H
#define COMMAND_H

#include "constants.h"

typedef struct
{
  char *argv[MAX_ARGS + 1];
  unsigned int argc;
  bool memory_error_flag;
  bool path_error_flag;
  bool background;
  // char history[]
  char *tokens[MAX_ARGS + 1];
  int input_fd;
  int output_fd;

} Command;

extern Command command;

char *get_command();
int run_command(Command *command, int input_fd, int output_fd);
void reset_command_struct(Command *command);
void flush();

#endif
