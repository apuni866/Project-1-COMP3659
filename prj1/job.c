#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"
#include "job.h"

void run_job(Job *job)
{
  int pipefd[2] = {1, 0};
  int child_status;
  int pid, pid2;

  int outfile = 1;
  int infile = 0;
  int stage = 0;

  char default_dir_path[MAX_BUFFER_SIZE] = "/bin/";
  char curr_dir_path[MAX_BUFFER_SIZE] = "/bin/";
  char *path = job->pipeline[stage].argv[0];
  strncat(curr_dir_path, path, get_strlen(path) + 1);
  char *const envp[] = {NULL};

  if (job->infile_path != NULL)
  {
    infile = open(job->infile_path, O_RDONLY);
    if (infile == -1)
    {
      write(STDERR_FILENO, "Error opening infile\n", 22);
      return;
    }
  }

  for (; stage < (job->num_stages - 1); stage++)
  {
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

      path = construct_cmd_path(job->pipeline[stage].argv[0], default_dir_path);
      execve(path, job->pipeline[stage].argv, envp);
      write(STDERR_FILENO, "Error executing command\n", 25);
      exit(EXIT_FAILURE);
    }

    close(pipefd[WRITE_END]);
    infile = pipefd[READ_END];
  }

  pid2 = fork();
  if (pid2 == 0)
  {
    if (job->outfile_path != NULL)
    {
      outfile = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (outfile == -1)
      {
        write(STDERR_FILENO, "Error opening outfile\n", 23);
        return;
      }
      dup2(outfile, 1);
      close(outfile);
    }

    if (infile != 0)
      dup2(infile, 0);

    path = construct_cmd_path(job->pipeline[stage].argv[0], default_dir_path);
    execve(path, job->pipeline[stage].argv, envp);
    write(STDERR_FILENO, "Error executing command\n", 25);
    exit(EXIT_FAILURE);
  }

  if (job->num_stages > 1)
  {
    close(pipefd[WRITE_END]);
    close(pipefd[READ_END]);
  }
  if (!job->background)
    waitpid(pid2, &child_status, 0);
  else
    write(STDOUT_FILENO, "Background process started\n", 28);
}

void reset_job(Job *job)
{

  job->infile_path = NULL;
  job->outfile_path = NULL;
  job->background = false;
  job->num_stages = 1;
  int i;

  for (i = 0; i < job->num_stages; i++)
    reset_command_struct(&job->pipeline[i]);
}

void open_output_file(Job *job, Command *command)
{
  command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG);
  if (command->output_fd == -1)
  {
    write(STDERR_FILENO, "Error opening outfile\n", 23);
    return;
  }
}
char *construct_cmd_path(const char *command, const char *default_dir_path)
{
  char *curr_dir_path = alloc(MAX_BUFFER_SIZE);
  if (curr_dir_path == NULL)
  {
    write(STDERR_FILENO, "Error allocating memory\n", 25);
    exit(EXIT_FAILURE);
  }
  strcpy(curr_dir_path, default_dir_path);
  strncat(curr_dir_path, command, get_strlen(command) + 1);
  return curr_dir_path;
}
/*******************************************************
 * @brief Prints the job structure for debugging purposes.
 */
void print_job(Job *job, char *message)
{
#if 0
  printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ JOB DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("Location:           %s\n", message);

  printf("Job has %d stage(s)\n", job->num_stages);
  printf("Input file:         %s\n", job->infile_path ? job->infile_path : "None");
  printf("Output file:        %s\n", job->outfile_path ? job->outfile_path : "None");
  printf("Background:         %s\n", job->background ? "Yes" : "No");

  for (unsigned int i = 0; i < job->num_stages; i++)
  {
    Command *command = &job->pipeline[i];
    printf("\n--- Stage %d ---\n", i + 1);
    printf("Command Arguments:  (argc = %d):\n", command->argc);
    for (unsigned int j = 0; j < command->argc; j++)
    {
      if (command->argv[j] != NULL)
      {
        printf("argv[%d]:            %s\n", j, command->argv[j]);
      }
      else
      {
        printf("argv[%d]:            NULL\n", j);
      }
    }

    // Print the last NULL pointer in argv
    if (command->argv[command->argc] == NULL)
    {
      printf("argv[%d]:            NULL\n", command->argc);
    }
  }

  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
#endif
}