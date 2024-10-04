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
  //char history[]
  
}Command;

extern Command command;

void get_command(Command* command);
int run_command(Command* command);
void reset_command_struct(Command* command);
void flush();


#endif
