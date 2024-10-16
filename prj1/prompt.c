#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"
#include "parse.h"

int create_job(Job *job, char input_str[MAX_BUFFER_SIZE]);

int main()
{
  Command command;
  Job job = {
      {{{NULL}, 0}},
      1,
      NULL,
      NULL,
      0};

  reset_command_struct(&command);

  while (true)
  {
    char *input_str = get_command();

    if (input_str == NULL || *input_str == '\0')
      continue;
    write(STDOUT_FILENO, input_str, MAX_BUFFER_SIZE - 1);
    if (create_job(&job, input_str) == -1)
      continue;

    if (string_compare(job.pipeline[0].argv[0], "exit", 4) == 0)
    {
      free_all();
      break;
    }

    if (string_compare(job.pipeline[0].argv[0], "clear", 5) == 0)
    {
      write(STDOUT_FILENO, "\033[H\033[J", 7);
      reset_command_struct(&command);
      free_all(); // Free input string after each command
      continue;
    }
    run_job(&job);

    free_all(); // Free input string after the job is run
    print_job(&job, "In main, after run_job()");
    reset_job(&job);
    print_job(&job, "In main, after reset_job()");

    // reset_command_struct(&command);
  }

  return 0;
}

int create_job(Job *job, char input_str[256])
{
  int len = get_strlen(input_str);
  int pipeline_index = 0;
  int argv_index = 0;
  bool space_found = false;
  bool pipeline_done = false;
  char sp_char;
  int i = 0;
  // printf("%d\n", len);

  while (input_str[i] == ' ' && i < MAX_BUFFER_SIZE)
  {
    printf("i: %d\n", i);
    i++;
  }
  if (input_str[i] == '\0' || input_str[i] == '\n')
  {
    printf("input_str is null, but thats bad :(\n");
    return -1;
  }
  job->pipeline[pipeline_index].argv[argv_index] = &input_str[0];
  job->pipeline[pipeline_index].argc = 1;
  argv_index++;

  job->num_stages = 1;
  for (; i < MAX_BUFFER_SIZE; i++)
  {
    if (!pipeline_done)
    {

      if (space_found && !(input_str[i] == '|' ||
                           input_str[i] == '<' ||
                           input_str[i] == '>' ||
                           input_str[i] == '&'))
      {
        space_found = false;
        job->pipeline[pipeline_index].argv[argv_index] = &input_str[i];
        job->pipeline[pipeline_index].argc++;
        argv_index++;
      }
      if (input_str[i] == '|')
      {
        space_found = false;
        input_str[i] = '\0';
        pipeline_index++;
        argv_index = 0;

        job->num_stages++;
        do
        {
          i++;
        } while (input_str[i] == ' ');
        job->pipeline[pipeline_index].argv[argv_index] = &input_str[i];
        job->pipeline[pipeline_index].argc = 1;
        argv_index++;
      }
      else if (input_str[i] == '<' ||
               input_str[i] == '>' ||
               input_str[i] == '&')
      {
        sp_char = input_str[i];
        input_str[i] = '\0';
        pipeline_done = true;
        space_found = false;
        job->pipeline[pipeline_index].argv[argv_index] = NULL;
      }
      else if (input_str[i] == ' ')
      {

        input_str[i] = '\0';
        space_found = true;
      }
      else if (input_str[i] == '\n' || input_str[i] == '\0')
      {
        job->pipeline[pipeline_index].argv[argv_index] = NULL;
        space_found = false;
        input_str[i] = '\0';
        break;
      }
    }
    else
    {
      if (input_str[i] == IO_IN || sp_char == IO_IN)
      {
        for (; input_str[i] == ' ' || input_str[i] == IO_IN; i++)
          input_str[i] = '\0';
        sp_char = ' ';
        job->infile_path = &input_str[i];
      }
      else if (input_str[i] == IO_OUT || sp_char == IO_OUT)
      {
        for (; input_str[i] == ' ' || input_str[i] == IO_OUT; i++)
          input_str[i] = '\0';
        sp_char = ' ';
        job->outfile_path = &input_str[i];
      }
      else if (input_str[i] == '&' || sp_char == '&')
      {
        input_str[i] = '\0';
        sp_char = ' ';
        job->background = true;
      }
      if (input_str[i] == '\n' || input_str[i] == ' ')
        input_str[i] = '\0';
    }
  }
  return 0;
}