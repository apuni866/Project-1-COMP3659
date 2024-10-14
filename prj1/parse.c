#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "command.h"
#include "job.h"
#include "constants.h"
#include "parse.h"

int tokenizer(char *input_buffer, Command *command)
{
  int counter = 0;
  char **args;
  args = command->tokens;

  // ignore white space
  while (*input_buffer == ' ')
    input_buffer++;

  // this main loop fills the argument array with the arguments found from the input buffer
  while (*input_buffer != '\0' && counter < MAX_ARGS)
  { // -1 ??
    // if switch case returns a delimit char skip lower 2 lines
    args[counter] = input_buffer;
    counter++;

    // move ptr along
    while (*input_buffer != ' ' && *input_buffer != '\0')
      *input_buffer++;

    // repace whitespace with a null
    if (*input_buffer == ' ')
    {

      *input_buffer = '\0';
      *input_buffer++;
    }
  }
  args[counter] = NULL; // null terminate this for execve(x,y,NULL) as it is a requirement (says in the manual 2 execve)
  return counter;
}

int parse(Command *command, Job *job)
{

  // char **temp = NULL; // for temp array
  int i = 0;
  char **tokens = command->tokens;

  if (tokens == NULL)
    return -1;

  while (tokens[i] != NULL && i < MAX_ARGS)
  {
    // printf("Under the tokens[i] != NULL cond'n \n");
    // printf("Tokens is: %s\n", tokens[i]);
    while (check_token(tokens, i) == REGULAR_TOKEN)
    {
      command->argv[i] = tokens[i];
      // printf(" At the top of check_token WHILE LOOP ----\n");
      // printf("tokens[i] = (%s),\n", tokens[i]);
      printf("argc: %d\n", command->argc);
      command->argc++;
      i++;
      printf("%d\n", i);
    }

    if (check_token(command->argv, 0) == SPECIAL_TOKEN)
      return -1;

    handle_special_token(command, job, &i);
    // printf("Under the handle specl token call inside parse\n");
  }

  // printf("leaving parse\n");
  return 0;
}
/*
void handle_special_token(Command *command, Job *job, int *i)
{
  char **tokens = command->tokens;
  if (tokens[*i] == NULL)
  {
    command->argv[*i] = NULL;
    return;
  }
  switch (*tokens[*i])
  {
  case PIPE:
    handle_pipeline(job, command);
    (*i)++;
    break;

  case IO_IN:
    (*i)++;
    break;

  case IO_OUT:
    // printArray(tokens, (sizeof(tokens) / sizeof(tokens[0])));

    if (tokens[*i + 1] != NULL)
    {
      printf("*i+1: %d, tokens[*i+1]: %s", *i + 1, tokens[*i + 1]);
    }
    else
    {
      printf("*i+1: %d, tokens[*i+1]: NULL", *i + 1);
    }

    handle_IO_output(job, command, i);
    break;

  case BACKGROUND:

    break;
  }
}
*/
void handle_special_token(Command *command, Job *job, int *i)
{
  char **tokens = command->tokens;
  if (tokens[*i] == NULL)
  {
    command->argv[*i] = NULL;
    return;
  }
  switch (*tokens[*i])
  {
  case PIPE:
    handle_pipeline(job, command);
    (*i)++;
    break;

  case IO_IN:
    handle_IO(job, command, i, IO_IN);

    break;

  case IO_OUT:
    // printArray(tokens, (sizeof(tokens) / sizeof(tokens[0])));

    if (tokens[*i + 1] != NULL)
    {
      printf("*i+1: %d, tokens[*i+1]: %s", *i + 1, tokens[*i + 1]);
    }
    else
    {
      printf("*i+1: %d, tokens[*i+1]: NULL", *i + 1);
    }

    handle_IO(job, command, i, IO_OUT);
    break;

  case BACKGROUND:

    break;
  }
}
int check_token(char **tokens, int i)
{
  if (tokens == NULL || tokens[i] == NULL)
  {
    return 0;
  }

  char *token = tokens[i];
  switch (*token)
  {
  case '\0':
    return -1;

  case PIPE:
    return SPECIAL_TOKEN;
  case IO_IN:
    return SPECIAL_TOKEN;
  case IO_OUT:
    return SPECIAL_TOKEN;
  case BACKGROUND:
    return SPECIAL_TOKEN;
  default:
    return REGULAR_TOKEN;
  }
}

void handle_pipeline(Job *job, Command *command)
{
  job->pipeline[job->num_stages] = *command;
  job->num_stages++;
}

void handle_IO(Job *job, Command *command, int *i, char io_char)
{
  char **tokens = command->tokens;

  if (tokens[*i + 1] != NULL)
  {
    if (io_char == IO_OUT)
      job->outfile_path = tokens[*i + 1];
    else
      job->infile_path = tokens[*i + 1];

    print_argv(command, job->outfile_path);
    tokens[*i + 1] = NULL;
  }

  *i += 2;
}