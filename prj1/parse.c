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
  if (tokens == NULL) return -1;
  printf("before while inside of parse\n");
  while(tokens != NULL) {
    //printf("outer while\n");
    while(check_token(tokens, i) == REGULAR_TOKEN) {
      //printf("inner while\n");
      command->argv[i] = tokens[i];
      //printf("*tokens[i] = (%c),\n",*tokens[i]);
      printf("tokens[i] = (%s),\n",tokens[i]);
      
      
      command->argc++;
      printf("under argc\n");
      i++;
    }
    //printf("henadle spec toekn bforew\n");
    handle_special_token(command, job, i);
  }

  printf("leaving parse\n");
  return 0;
}
void handle_special_token(Command* command, Job* job, int i) {
  char** tokens = command->tokens;
  if (tokens[i] == NULL) {
    command->argv[i] = '\0';
    return;
  }
  printf("handle_spec_char before *token[i]\n");
  switch(*tokens[i]) {
    case PIPE: 
    printf("\nhandle_pipe()...\n");
    //handle_pipe(job);
    break;
    case IO_IN: 
    case IO_OUT:
    //handle_io_redirect(command);
    break;
    case BACKGROUND:
    //handle_background(idfk);
    break;
  }
}
int check_token(char** tokens, int i) {
  //printf("enter chk tokn\n");
  char *token = tokens[i];
  if (token == NULL) return 0;
  //printf("token is: %c",*token);
  switch (*token) {
    case '\0':   return -1; break;
    case PIPE: return SPECIAL_TOKEN; break;
    case IO_IN: return SPECIAL_TOKEN; break;
    case IO_OUT: return SPECIAL_TOKEN; break;
    case BACKGROUND: return SPECIAL_TOKEN; break;
    default: return REGULAR_TOKEN;
  }

}