#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "command.h"
#include "job.h"
#include "constants.h"
#include "parse.h"


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
  return counter;
}

int parse(Command *command, Job *job)
{

  //find ls -al | now we want to put ls -al into pipeline[i] and consume pipe char |

  char** temp = NULL; //for temp array
  int i = 0;
  char** tokens = command->tokens; 

  if (tokens == NULL) 
    return -1;

  while(tokens[i] != NULL && i < MAX_ARGS)  
  {
    printf("Under the tokens[i] != NULL cond'n \n");
    while(check_token(tokens, i) == REGULAR_TOKEN) 
    {
      command->argv[i] = tokens[i];
      printf(" At the top of check_token WHILE LOOP ----\n");

      //printf("*tokens[i] = (%c),\n",*tokens[i]);
      printf("tokens[i] = (%s),\n",tokens[i]);
      
      command->argc++;
      //printf("under argc\n");
      i++;
    }

    if (check_token(command->argv, 0) == SPECIAL_TOKEN)
      return -1;

    handle_special_token(command, job, &i);
    printf("Under the handle specl token call inside parse\n");
  }

  printf("leaving parse\n");
  return 0;

}
void handle_special_token(Command* command, Job* job, int *i) 
{ 
  char** tokens = command->tokens;
  if (tokens[*i] == NULL) { 
    command->argv[*i] = NULL;
    return;
  }
  switch(*tokens[*i]) 
  {
    case PIPE: 
    printf("\nhandle_pipe()...\n");
    handle_pipeline(job,command);
    (*i)++;
    break;

    case IO_IN: 
    (*i)++;
    break;

    case IO_OUT:
    //job->outfile_path = tokens[++(*i)];          //skip to the file name from the special_char
    //job->pipeline[job->num_stages] = *command;
    handle_IO_output(job,command,i);
    break;

    case BACKGROUND:
    //handle_background(idfk);
    break;
  }
}
int check_token(char** tokens, int i) 
{
  printf(" At the top of check_token\n");
  if (tokens == NULL || tokens[i] == NULL) { 
    printf("Inside of the if tokens[i] == NULL\n");
    return 0;
  }

  char *token = tokens[i];


  printf("RIGHT before doing *token inside of check_token\n");
  switch (*token) {
    case '\0':   return -1;
    
    case PIPE: 
    return SPECIAL_TOKEN;
    printf("Inside of PIPE switch case\n"); 
    
    case IO_IN: return SPECIAL_TOKEN;
    case IO_OUT:
    printf("Inside fo IO_OUT switch case\n"); 
    return SPECIAL_TOKEN;
    case BACKGROUND: return SPECIAL_TOKEN;
    default: return REGULAR_TOKEN;
  }
  
}

void handle_pipeline(Job *job, Command *command)
{
  job->pipeline[job->num_stages] = *command;    //assign it the entire command struct till this point 
  job->num_stages++;

  //reset command struct here ?

}

void handle_IO_output(Job *job, Command *command, int *i)
{
  printf("Inside of handle_IO_output (at the top)\n");
  char** tokens = command->tokens;

  if (tokens[*i + 1] != NULL ) {
    printf(" This is the value of tokens: %s\n", tokens[*i + 1]);
    job->outfile_path = tokens[*i + 1];
  }
  
  *i += 2;

}