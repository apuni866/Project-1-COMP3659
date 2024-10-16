#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"

#include "parse.h"

#include <string.h>

char *get_command()
{
  char *buffer = alloc(MAX_BUFFER_SIZE + 1); // Allocate memory for the input buffer
  int bytesRead;
  write(STDOUT_FILENO, PROMPT_SYMBOL, PROMPT_SYMBOL_SIZE);

  bytesRead = read(STDIN_FILENO, buffer, MAX_BUFFER_SIZE - 1); // Leave space for null terminator
  // printf("%d", errno);
  if (bytesRead < 0)
  {
    printf("This should never happen, because just think about it\n");
    free_all();  // Clean up the buffer
    return NULL; // Return NULL in case of error
  }

  else if (bytesRead == 0)
  {
    // read returned EOF
    free_all();
    return '\0';
  }

  else if (bytesRead >= MAX_BUFFER_SIZE - 1)
  {
    flush(); // Clear the buffer if too large
  }

  buffer[bytesRead - 1] = '\0'; // Null-terminate the string (removing newline)

  // printf("This is the string in the buffer: %s\n", buffer);
  //  int i = 0;
  //  while (buffer[i] != '\0')
  //    i++;
  //  printf("Null terminator encountered \n");

  return buffer;
}
/*
int run_command(Command *command, int input_fd, int output_fd)
{

  int child_status;
  int pid;

  char *const envp[] = {NULL};
  char *const path = command->argv[0];           // direct path
  char curr_dir_path[MAX_BUFFER_SIZE] = "/bin/"; // current directory path.
  strncat(curr_dir_path, path, get_strlen(path) + 1);

  pid = fork();

  if (pid == -1)
  {
    perror("Could not fork.\n");
    exit(EXIT_FAIL);
  }

  if (pid == 0)
  {

    if (output_fd != INIT_VALUE)
    {
      // print_argv(command, "in run_command(), above dup2()");
      if (dup2(output_fd, STDOUT_FILENO) == -1)
      {
        print_argv(command, "in run_command(), dup2() returned -1");
        perror("Failed to redirect stdout.\n");
        close(output_fd);
        return 0;
      }
      close(output_fd);
      // close(output_fd);
    }
    if (input_fd != INIT_VALUE)
    {
      if (dup2(input_fd, STDIN_FILENO) == -1)
      {
        perror("Failed to redirect stdin.\n");
        close(input_fd);
        return 0;
      }
      close(input_fd);
    }
    // if (execve(path,command->argv,envp)== -1)
    if (execve(curr_dir_path, command->argv, envp) == -1)
    // if (execve(curr_dir_path, command->argv, NULL) == -1)
    { // this works by itself no need for upper if block
      perror("The execeve AAAAA could not open process");
      exit(EXIT_FAIL);
    }

    // dup2(original_stdout,STDOUT_FILENO);
    // close(original_stdout);
  }
  else
  {
    waitpid(pid, &child_status, 0);
  }

  return 0;
}
*/
void reset_command_struct(Command *command)
{
  command->argc = 0;
  // command->input_fd = -1;
  // command->output_fd = -1;
  free_all();
  // flush();

  for (int i = 0; i < MAX_ARGS; i++)
  {
    command->argv[i] = NULL;
  }
}

void flush()
{
  char discard;

  // printf("Flushing ...\n");

  while (read(STDIN_FILENO, &discard, 1) > 0 && discard != '\n')
    ;
}
