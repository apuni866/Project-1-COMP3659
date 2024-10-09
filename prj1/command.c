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

void get_command(Command* command){
  //char buffer[MAX_BUFFER_SIZE]; // Buffer to hold the command line input
  int bytesRead;
  int index = 0;
  char *buffer = alloc(MAX_BUFFER_SIZE);  //max_buffer_size = 256  

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


int run_command(Command* command)
{

  int child_status;
  int pid;
  //char * const envp[] = {NULL};
  char * envp[] = {(char *)"PATH=/bin",0};
  char * const path = command->argv[0]; //direct path
  char curr_dir_path[MAX_BUFFER_SIZE] = "/bin/"; //current directory path.
  strncat(curr_dir_path, path, get_strlen(path)+ 1);


  //we'll add pipe here later / io redirect later
  
  pid = fork();
  
  if (pid == -1){
    perror("Could not fork.\n");
    exit(EXIT_FAIL);
  }

  
  if(pid ==0)
  {

    //  if (input_fd != STDIN_FILENO)
    // {
    //   dup2(input_fd, STDIN_FILENO);
    //   close(input_fd);
    // }
    // if (output_fd != STDOUT_FILENO)
    // {
    //   dup2(output_fd, STDOUT_FILENO);
    //   close(output_fd);
    // }


    //attempt to open direct path and then attempt current directory.
    // mostly just for convenient for testing later.
    //if (execve(path,command->argv,envp)== -1)
      if (execve(curr_dir_path, command->argv, envp) ==-1){  //this works by itself no need for upper if block
	      perror("Could not open process");
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
  command->background = false;
  command->argv[0] = NULL;
  //free_all();
}

void flush()
{
  char discard;
  
  printf("Flushing ...\n");
  
  while( read(STDIN_FILENO, &discard, 1) > 0 && discard != '\n')
    ;
  
}

/*-----
int handle_IO_redirection()
{
}*/