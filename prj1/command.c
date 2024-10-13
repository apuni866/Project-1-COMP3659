#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"

#include "parse.h"

#include <string.h>

void get_command(Command* command){
  //char buffer[MAX_BUFFER_SIZE]; // Buffer to hold the command line input
  unsigned int bytesRead;
  int index = 0;
  char *buffer = alloc(MAX_BUFFER_SIZE + 1);  

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
    }

  buffer[bytesRead-1] = '\0';                // Null terminate here as within the 'parse' func'n a null check is done for this buffer

  /*parse(buffer, command->argv);*/            /* Pass in the command->argv as 'parse' populates the argument
					        vector with the pointer to the location of the argument string 
					        this means in the main (prompt.c) we can just see if command.argv[0] == "exit" */
  
  printf("calling tokenizer\n");
  tokenizer(buffer, command);   
 
  
  return;
}


int run_command(Command* command, int input_fd, int output_fd)
{

  int child_status;
  int pid;
  
  char * const envp[] = {NULL};
  //char * envp[] = {(char *)"PATH=/bin/",NULL};
  char * const path = command->argv[0]; //direct path
  char curr_dir_path[MAX_BUFFER_SIZE] = "/bin/"; //current directory path.
  strncat(curr_dir_path, path, get_strlen(path)+ 1);

  pid = fork();
  
  if (pid == -1){
    perror("Could not fork.\n");
    exit(EXIT_FAIL);
  }

  
  else if(pid == 0)
  { 

    //printf("BEFORE----the output != INIT_VAL------\n");
    if (command->output_fd != INIT_VALUE)
    {
      if ( dup2(command->output_fd, STDOUT_FILENO) == -1 )
      {
        perror("Failed to redirect stdout.\n");
        close(command->output_fd);
        exit(EXIT_FAIL);
      }
      //write(1,"Before close\n",14);
      close(command->output_fd);
    }
    if (command->input_fd != INIT_VALUE)
    { 
      if (dup2(command->input_fd,STDIN_FILENO) == -1)
      {
        perror("Failed to redirect stdin.\n");
        close(command->input_fd);
        exit(EXIT_FAIL);
      }
      
      close(command->input_fd);

    }
    

    //if (execve(path,command->argv,envp)== -1)
      // for (int j = 0; command->argv[j] != NULL; j++) {
      // printf("argv[%d]: %s (length: %zu)\n", j, command->argv[j], strlen(command->argv[j]));
      // }
      if (execve(curr_dir_path, command->argv, envp) == -1){  //this works by itself no need for upper if block
	      perror("The execeve could not open process");
	      exit(EXIT_FAIL);
      }
	  
  }
  else{
    waitpid(pid, &child_status, 0); 
  }
  
  
  return 0;
}


void reset_command_struct(Command* command)
{
  command->argc = 0;
  command->argv[0] = NULL;
  //command->input_fd = -1;
  //command->output_fd = -1;
  free_all();
  //flush();
}

void flush()
{
  char discard;
  
  printf("Flushing ...\n");
  
  while( read(STDIN_FILENO, &discard, 1) > 0 && discard != '\n')
    ;


}

//also add elwse if bytesRead == 1 
//    do something here to handle that 'enter' key press
// void validate_bytesRead(unsigned int bytesRead)
// {



// }