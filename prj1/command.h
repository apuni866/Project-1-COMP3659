#ifndef COMMAND_H
#define COMMAND_H

#include "constants.h"


typedef struct
{
  char *argv[MAX_ARGS + 1];
  unsigned int argc;

  // . . .

}Command;


get_command(Command &command);


#endif
