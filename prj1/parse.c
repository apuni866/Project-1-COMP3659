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
  int j = 0;

  char** tokens = command->tokens; 

  if (tokens == NULL) 
    return -1;

  while(tokens[i] != NULL ) {

    while(check_token(tokens, i) == REGULAR_TOKEN) {
      command->argv[i] = tokens[i];
      //printf("*tokens[i] = (%c),\n",*tokens[i]);
      //printf("tokens[i] = (%s),\n",tokens[i]);
      
      command->argc++;
      //printf("under argc\n");
      i++;
    }
    //printf("henadle spec toekn bforew\n");
    handle_special_token(command, job, &i);
  }

  //printf("leaving parse\n");
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
    job->infile_path = tokens[++(*i)];
    (*i)++;
    break;

    case IO_OUT:
    job->outfile_path = tokens[++(*i)];          //skip to the file name from the special_char
    (*i)++;                                       //skip file name as we dont want it in the argv
    break;

    case BACKGROUND:
    //handle_background(idfk);
    break;
  }
}
int check_token(char** tokens, int i) 
{
  char *token = tokens[i];
  if (token == NULL) return 0;
  switch (*token) {
    case '\0':   return -1; break;
    case PIPE: return SPECIAL_TOKEN; break;
    case IO_IN: return SPECIAL_TOKEN; break;
    case IO_OUT: return SPECIAL_TOKEN; break;
    case BACKGROUND: return SPECIAL_TOKEN; break;
    default: return REGULAR_TOKEN;
  }

}

void handle_pipeline(Job *job, Command *command)
{
  job->pipeline[job->num_stages] = *command;    //assign it the entire command struct till this point 
  job->num_stages++;

}