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

#if 0
void allocate_jobs(Job *job, Command *command)
{
  if (job->num_stages > 1)
  {
    run_job(job);
    return;
  }
  else
  {
    if (job->outfile_path != NULL)
    {
      command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG);
      if (command->output_fd == -1)
      {
        perror("Something went wrong with opening the specified file.\n");
        return;
      }
    }
    if (job->infile_path != NULL)
    {
      command->input_fd = open(job->infile_path, O_RDONLY);
      if (command->input_fd == -1)
      {
        perror("This file does not exist.\n");
        return;
      }
    }

    // print_argv(command);

    // printf("Calling run_command...\n");
    print_argv(command, "inside run_job(), above run_command()");
    run_command(command, command->input_fd, command->output_fd);
  }
  // reset_job(job);
  reset_command_struct(command);
}
void run_job(Job *job)
{
  int pipefd[2];
  int child_status;
  int pid, pid2;
  char *const envp[] = {NULL};
  int outfile;
  int infile = 0;
  int new_in;
  int stage = 0;

  if (job->infile_path)
  {
    infile = open(job->infile_path, O_RDONLY, 0644);
    printf("infile: %s: %d\n", job->infile_path, infile);
  }
  for (; stage < (job->num_stages - 1); stage++)
  {
    write(1, "loop\n", 6);
    printf("stage: %d, job->num_stage: %d\n", stage, job->num_stages);
    pipe(pipefd);

    pid = fork();

    if (pid == 0)
    {

      if (infile != 0)
      {
        dup2(infile, 0);
        close(infile);
      }

      close(pipefd[READ_END]);
      dup2(pipefd[WRITE_END], 1);
      close(pipefd[WRITE_END]);

      execve(job->pipeline[stage].argv[0], job->pipeline[stage].argv, envp);
      exit(EXIT_SUCCESS); // safetey exit. replace with error trap instead later.
    }

    close(pipefd[WRITE_END]);
    infile = pipefd[READ_END];
  }

  pid2 = fork();
  if (pid2 == 0)
  {

    if (job->outfile_path)
    {
      outfile = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      printf("outfile: %s: %d\n", job->outfile_path, outfile);
      dup2(outfile, 1);
      close(outfile);
    }

    close(pipefd[WRITE_END]);
    dup2(pipefd[READ_END], 0);

    execve(job->pipeline[stage].argv[0], job->pipeline[stage].argv, envp);
  }

  close(pipefd[READ_END]);
  close(pipefd[WRITE_END]);

  if (!job->background)
  {
    // waitpid(pid, &child_status, 0);
    waitpid(pid2, &child_status, 0);
  }
  else
    printf("hey its a backgroundjob");

  reset_job(job);
}
#endif

void run_job(Job *job)
{
  int pipefd[2] = {1, 0};
  int child_status;
  int pid, pid2;

  int outfile = 1;
  int infile = 0;
  int stage = 0;
  char curr_dir_path[MAX_BUFFER_SIZE] = "/bin/"; // current directory path.
  char *const path = job->pipeline[stage].argv[0];
  strncat(curr_dir_path, path, get_strlen(path) + 1);
  printf("curr_dir_path: %s", curr_dir_path);
  char *const envp[] = {NULL};

  if (job->infile_path != NULL)
  {
    infile = open(job->infile_path, O_RDONLY);
    printf("infile: %s: %d\n", job->infile_path, infile);
    dup2(infile, 0);
    close(infile);
  }

  for (; stage < (job->num_stages - 1); stage++)
  {
    write(1, "loop\n", 6);
    pipe(pipefd);

    pid = fork();

    if (pid == 0)
    {
      if (infile != 0)
      {
        dup2(infile, 0);
        close(infile);
      }

      close(pipefd[READ_END]);
      dup2(pipefd[WRITE_END], 1);
      close(pipefd[WRITE_END]);

      execve(job->pipeline[stage].argv[0], job->pipeline[stage].argv, envp);

      exit(EXIT_SUCCESS); // safetey exit. replace with error trap instead later.
    }

    close(pipefd[WRITE_END]);
    infile = pipefd[READ_END];
  }

  pid2 = fork();
  printf("pid2: %d\n", pid2);
  if (pid2 == 0)
  {

    if (job->outfile_path != NULL)
    {
      outfile = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      printf("outfile: %s: %d\n", job->outfile_path, outfile);
      dup2(outfile, 1);
      close(outfile);
    }

    if (infile != 0)
      dup2(infile, 0);
    if (outfile != 1)
    {
      // dup2(pipefd[READ_END],0);
    }

    printf("This is the string insdie of run_job: %s\n", job->pipeline[stage].argv[0]);

    // execve("/bin/ls", job->pipeline[stage].argv, envp);
    //  ^^^ THIS WORKS!!
    execve(curr_dir_path, job->pipeline[stage].argv, envp);
  }
  if (job->num_stages > 1)
  {
    close(pipefd[WRITE_END]);
    close(pipefd[READ_END]);
  }
  if (!job->background)
    waitpid(pid2, &child_status, 0);
  else
    printf("hey its a backgroundjob");
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
  job->num_stages = 1;
  /*
    job =
        {
            {{{NULL}, 0}},
            1,
            NULL,
            NULL,
            0};
            */
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
#if 1
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
#endif
}
void initialize_job(Job *job)
{
  /*
  job =
      {
          {{{"/usr/bin/ls", "-al", "/library", NULL}, 3}},
          1,
          NULL,
          NULL,
          0};
          */
  reset_command_struct(&(job->pipeline[0]));
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