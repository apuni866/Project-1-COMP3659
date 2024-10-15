#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"
#include "parse.h"

void add_argument_to_pipeline(Job *job, int pipeline_index, int *argv_index, char *input_str, int pos);
void start_new_pipeline_stage(Job *job, int *pipeline_index, int *argv_index, char *input_str, int *pos);
void handle_special_char(Job *job, char *input_str, int pos, char *sp_char, bool *pipeline_done);
void set_input_output_paths(Job *job, char *input_str, int *pos, char sp_char);
void create_job(Job *job, char input_str[MAX_BUFFER_SIZE]);

int main()
{
  Command command;
  Job job =
      {
          {{{NULL}, 0}},
          1,
          NULL,
          NULL,
          0};

  reset_command_struct(&command);
  // job.infile_path = NULL;
  // job.outfile_path = NULL;

  while (true)
  {
    char *input_str = get_command();

    if (input_str == NULL)
    {
      break; // If there was an error or EOF, exit the loop
    }

    create_job(&job, input_str);
    printf("Left create job\n");
    print_argv(&job.pipeline[0], "After create_job");

    if (string_compare(job.pipeline[0].argv[0], "exit", 4) == 0)
    {
      free_all(); // Free the input string memory before exiting
      break;
    }

    if (string_compare(job.pipeline[0].argv[0], "clear", 5) == 0)
    {
      write(STDOUT_FILENO, "\033[H\033[J", 7); // ANSI escape codes to clear the screen
      reset_command_struct(&command);
      free_all(); // Free input string after each command
      continue;
    }

    run_job(&job);

    free_all(); // Free input string after the job is run
    reset_job(&job);
    // reset_command_struct(&command);
  }

  return 0;
}
void add_argument_to_pipeline(Job *job, int pipeline_index, int *argv_index, char *input_str, int pos)
{
  job->pipeline[pipeline_index].argv[*argv_index] = &input_str[pos];
  job->pipeline[pipeline_index].argc++;
  (*argv_index)++;
}

void start_new_pipeline_stage(Job *job, int *pipeline_index, int *argv_index, char *input_str, int *pos)
{
  input_str[*pos] = '\0';
  (*pipeline_index)++;
  *argv_index = 0;
  job->num_stages++;

  // Skip over spaces
  do
  {
    (*pos)++;
  } while (input_str[*pos] == ' ');

  job->pipeline[*pipeline_index].argv[*argv_index] = &input_str[*pos];
  job->pipeline[*pipeline_index].argc = 1;
  (*argv_index)++;
}

void handle_special_char(Job *job, char *input_str, int pos, char *sp_char, bool *pipeline_done)
{
  *sp_char = input_str[pos];
  input_str[pos] = '\0';
  *pipeline_done = true;
  job->pipeline[job->num_stages - 1].argv[job->pipeline[job->num_stages - 1].argc] = NULL; // Terminate current pipeline stage
}

void set_input_output_paths(Job *job, char *input_str, int *pos, char sp_char)
{
  if (sp_char == IO_IN || input_str[*pos] == IO_IN)
  {
    for (; input_str[*pos] == ' ' || input_str[*pos] == IO_IN; (*pos)++)
    {
      input_str[*pos] = '\0';
    }
    job->infile_path = &input_str[*pos];
  }
  else if (sp_char == IO_OUT || input_str[*pos] == IO_OUT)
  {
    for (; input_str[*pos] == ' ' || input_str[*pos] == IO_OUT; (*pos)++)
    {
      input_str[*pos] = '\0';
    }
    job->outfile_path = &input_str[*pos];
  }
  else if (sp_char == '&')
  {
    input_str[*pos] = '\0';
    job->background = true;
  }
}
void create_job(Job *job, char input_str[256])
{
  int len = get_strlen(input_str);
  int pipeline_index = 0;
  int argv_index = 0;
  bool space_found = false;
  bool pipeline_done = false;
  char sp_char;
  // printf("%d\n", len);

  job->pipeline[pipeline_index].argv[argv_index] = &input_str[0];
  job->pipeline[pipeline_index].argc = 1;
  argv_index++;

  job->num_stages = 1;
  for (int i = 0; i < MAX_BUFFER_SIZE; i++)
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
      else if (input_str[i] == '\n')
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
  return;
}
#if 0
void create_job(Job *job, char input_str[MAX_BUFFER_SIZE])
{
  int pipeline_index = 0;
  int argv_index = 0;
  bool space_found = false;
  bool pipeline_done = false;
  char sp_char;

  job->pipeline[pipeline_index].argv[argv_index] = &input_str[0];
  // printf("This is the string insdie of the job->pipe: %s\n", job->pipeline[pipeline_index].argv[argv_index]);

  job->pipeline[pipeline_index].argc = 1;
  argv_index++;

  job->num_stages = 1;

  // -1 ??
  for (int i = 0; i < MAX_BUFFER_SIZE; i++)
  {
    if (!pipeline_done)
    {
      if (space_found && !(input_str[i] == '|' || input_str[i] == '<' || input_str[i] == '>' || input_str[i] == '&'))
      {
        space_found = false;
        add_argument_to_pipeline(job, pipeline_index, &argv_index, input_str, i);
      }
      if (input_str[i] == '|')
      {
        space_found = false;
        start_new_pipeline_stage(job, &pipeline_index, &argv_index, input_str, &i);
      }
      else if (input_str[i] == '<' || input_str[i] == '>' || input_str[i] == '&')
      {
        handle_special_char(job, input_str, i, &sp_char, &pipeline_done);
      }
      else if (input_str[i] == ' ')
      {
        input_str[i] = '\0';
        space_found = true;
      }
      else if (input_str[i] == '\n')
      {
        job->pipeline[pipeline_index].argv[argv_index] = NULL;
        space_found = false;
        input_str[i] = '\0';
        break;
      }
      // else if (input_str[i] == '\0')
      //   printf("INside of else if input_str == null termionator \n");
    }
    else
    {
      set_input_output_paths(job, input_str, &i, sp_char);
      if (input_str[i] == '\n' || input_str[i] == ' ')
      {
        input_str[i] = '\0';
      }
    }
  }
}
#endif