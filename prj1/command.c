#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"

void get_command(Command* command){
  char buffer[MAX_BUFFER_SIZE]; // Buffer to hold the command line input
  int bytesRead;
  int index = 0;
    
  reset_command_struct(command);
  write(STDOUT_FILENO, PROMPT_SYMBOL, PROMPT_SYMBOL_SIZE); // Write prompt to standard output

  // Read command line
  bytesRead = read(STDIN_FILENO, buffer, MAX_BUFFER_SIZE - 1); // Leave space for null terminator

  if (bytesRead <= 0) {
    // This should never happen because just think about it 
    command->argv[0] = NULL;
    return;
  }

  
  buffer[bytesRead] = '\0';                // Null terminate here as within the 'parse' func'n a null check is done for this buffer
  write(STDOUT_FILENO, buffer, bytesRead); // Write prompt to standard output
  parse(buffer, command->argv);            /* Pass in the command->argv as 'parse' populates the argument
					        vector with the pointer to the location of the argument string 
					        this means in the main (prompt.c) we can just see if command.argv[0] == "exit" */
 
 
  
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
