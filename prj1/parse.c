#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "command.h"
#include "job.h"
#include "constants.h"


int tokenizer(char *input_buffer, Command* command)
{
  int counter = 0;
  char **args; 
  args = command->tokens;



  //ignore white space
  while(*input_buffer == ' ')
  input_buffer++;


  //this main loop fills the argument array with the arguments found from the input buffer
  while(*input_buffer != '\0' && counter < MAX_ARGS) { // -1 ??
  //if switch case returns a delimit char skip lower 2 lines
    args[counter] = input_buffer;
    counter++;

    //move ptr along
    while(*input_buffer != ' ' && *input_buffer != '\0')
      *input_buffer++;


    //repace whitespace with a null
    if (*input_buffer == ' '){

      *input_buffer = '\0';
      *input_buffer++;

      }
  }
  args[counter] = NULL; //null terminate this for execve(x,y,NULL) as it is a requirement (says in the manual 2 execve)

  command->argc = counter;

    printf("tokenizer returns\n");
    
  return counter;

}

void parse(Command *command, Job *job)
{

  //find ls -al | now we want to put ls -al into pipeline[i] and consume pipe char |



}