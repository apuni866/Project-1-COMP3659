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
      //*input_buffer++;

      // repace whitespace with a null
      if (*input_buffer == ' ')
      {
        *input_buffer = '\0';
        //*input_buffer++;
      }
  }
  args[counter] = NULL; // null terminate this for execve(x,y,NULL) as it is a requirement (says in the manual 2 execve)
  return counter;
}

int parse(Command *command, Job *job)
{
  int i = 0;
  int j = 0;
  char **tokens = command->tokens;
  int pipeline_index = 0;

  if (tokens == NULL)
    return -1;

  while (tokens[i] != NULL && i < MAX_ARGS)
  {
    while (check_token(tokens, i) == REGULAR_TOKEN)
    {
      job->pipeline[pipeline_index].argv[j] = tokens[i];
      print_argv(&job->pipeline[pipeline_index], ":D");
      job->pipeline[pipeline_index].argc++;
      i++;
      j = i;
    }

    if (check_token(command->argv, 0) == SPECIAL_TOKEN)
      return -1;

    handle_special_token(command, job, &i, &j, &pipeline_index);
    // printf("Under the handle specl token call inside parse\n");
  }

  // printf("leaving parse\n");
  return 0;
}
void handle_special_token(Command *command, Job *job, int *i, int *j, int *pipe_index)
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
    (*pipe_index)++;
    *j = 0;
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
  // job->pipeline[job->num_stages - 1] = *command;
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