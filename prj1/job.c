#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <stdio.h> //remove this later

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"
#include "job.h"

void run_job(Job *job, Command *command)
{
  if (job->num_stages > 1)
  {
    perror("Error! More than 1 pipe was detected\n");
    job->num_stages = 0;

    return;
  }

  if (job->outfile_path != NULL)
  {
    // printf("Inisde of job != outfile path\n");

    command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG);
    if (command->output_fd == -1)
    {
      perror("Something went wrong with opening the specified file.\n");
      return;
    }
  }

  // print_argv(command);

  // printf("Calling run_command...\n");
  print_argv(command, "inside run_job(), above run_command()");
  run_command(command, command->input_fd, command->output_fd);
  reset_job(job);
  reset_command_struct(command);
}

void reset_job(Job *job)
{
  job->infile_path = NULL;
  job->outfile_path = NULL;
  int i;

  for (i = 0; i < job->num_stages; i++)
  {
    reset_command_struct(&job->pipeline[i]);
  }
  job->num_stages = 0;
}

void open_output_file(Job *job, Command *command)
{
  command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG);
  if (command->output_fd == -1)
  {
    perror("Something went wrong with opening the specified file.\n");
    return;
  }
}

void print_argv(Command *command, char *message)
{
  int temp_out_fd = command->output_fd;
  command->output_fd = STDOUT_FILENO;
  printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARGUMENT VECTOR DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("Location:           %s\n", message);

  printf("Command Arguments:  (argc = %d):\n", command->argc);
  printf("Command input fd:   %d\n", command->input_fd);
  printf("Command output fd:  %d\n", temp_out_fd);
  for (unsigned int i = 0; i < command->argc; i++)
  {
    if (command->argv[i] != NULL)
    {
      printf("argv[%d]:            %s\n", i, command->argv[i]);
    }
    else
    {
      printf("argv[%d]:           NULL\n", i);
    }
  }

  // Print the last NULL pointer in argv
  if (command->argv[command->argc] == NULL)
  {
    printf("argv[%d]:            NULL\n", command->argc);
  }
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  // resets the output fd to whatever it was before
  command->output_fd = temp_out_fd;
}

void printArray(char *arr[], size_t size)
{
  printf("Array elements: [");
  for (int i = 0; i < size; i++)
  {
    printf("%s", arr[i]);
    if (i < size - 1)
    {
      printf(", "); // Separate elements with a comma and space
    }
  }
  printf("]\n"); // Close the array format and add a newline
}