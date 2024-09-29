#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"

void get_command(Command* command){
  //char buffer[MAX_BUFFER_SIZE]; // Buffer to hold the command line input
  int bytesRead;
  int index = 0;
  char *buffer = alloc(MAX_BUFFER_SIZE);  //max_buffer_size = 256  

  //reset_command_struct(command);
  write(STDOUT_FILENO,"\n",1);
  write(STDOUT_FILENO, PROMPT_SYMBOL, PROMPT_SYMBOL_SIZE); // CMD line Symbol 

  // Read command line
  bytesRead = read(STDIN_FILENO, buffer, MAX_BUFFER_SIZE - 1); // Leave space for null terminator

  if (bytesRead <= 0){
    // This should never happen because just think about it
    command->argv[0] = NULL;
    return;
  }
 
  else if (bytesRead >= MAX_BUFFER_SIZE - 1){
      flush();
      printf("flush is was called\n");
	     
    /*printf("\n--->%d\n",bytesRead);
    char *new_buffer = resize(buffer);
    if (new_buffer == NULL){
     command->memory_error_flag = true;
      write(STD_FILENO, "Failed to allocate more memory\n" 31);
      return;
    }

    else{
      buffer = new_buffer;
      printf("\nSuccess in allocating more memory inside of command.c\n\n");
      printf("sizeof new buffer%lu\n",sizeof(new_buffer));
      }*/
    }

  printf("\nNormal bytes read from above line--->%d\n",bytesRead);
  buffer[bytesRead-1] = '\0';                // Null terminate here as within the 'parse' func'n a null check is done for this buffer
  write(STDOUT_FILENO, buffer, bytesRead); // Write prompt to standard output
  parse(buffer, command->argv);            /* Pass in the command->argv as 'parse' populates the argument
					        vector with the pointer to the location of the argument string 
					        this means in the main (prompt.c) we can just see if command.argv[0] == "exit" */

  reset_command_struct(command);
  
  
  return;
}

int run_command(Command* command){

  int pid;
  
  //if argv[0] is an invalid executable, it shoudlnt fork so we should validate input in get command
  pid = fork();
  if(pid ==0){


    if (command->argc ==1)
      execve(command->argv,NULL,NULL);
    else {
      execve(command->argv[0],command->argv, NULL);
    }
    
  }
  else{
    wait(pid);
    
    
  }




  
  return 0;
}

/******
 *
 ******/
void reset_command_struct(Command* command){
  command->argc = 0;
  command->background = false;
  //free_all();
}

void flush()
{
  char discard;
  while( read(STDIN_FILENO, &discard, 1) > 0 && discard != '\n')
    {
      printf("Flushing ...\n");
    }
  
}
