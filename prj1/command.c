#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"

void get_command(Command* command){
  //  char buffer[MAX_BUFFER_SIZE]; // Buffer to hold the command line input
    int bytesRead;
    int index = 0;
    
    reset_command_struct(command);
    write(STDOUT_FILENO, PROMPT_SYMBOL, PROMPT_SYMBOL_SIZE); // Write prompt to standard output

    // Read command line
    bytesRead = read(STDIN_FILENO, command->argv, MAX_BUFFER_SIZE - 1); // Leave space for null terminator

    if (bytesRead <= 0) {
      // This should never happen because just think about it, idiot. -_-. skill issue
      command->argv[0] = NULL;
      return;
    }
    write(STDOUT_FILENO, command->argv, bytesRead); // Write prompt to standard output
    return;
}

int run_command(Command* command){
  return 0;
}
void reset_command_struct(Command* command){
  command->argc = 0;
  command->background = false;
  free_all();
}
