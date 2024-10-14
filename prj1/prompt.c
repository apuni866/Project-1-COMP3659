#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"
#include "parse.h"

int main()
{
  Command command;
  Job job;
  command.memory_error_flag = false;
  reset_command_struct(&command);
  job.infile_path = '\0';
  job.outfile_path = '\0';
  job.num_stages = 1;

  while (true)
  {
    get_command(&command);
    // parse(&command,&job);

    // printf("IN main loop under the parse call\n");
    if (parse(&command, &job) == -1)
    {
      reset_command_struct(&command);
      continue;
    }
    print_argv(&command, "Right After parse");

    if (string_compare(command.argv[0], "exit", 4) == 0)
      break;

    // clear command works

    if (string_compare(command.argv[0], "clear", 5) == 0)
    {
      write(STDOUT_FILENO, "\033[H\033[J", 7);
      reset_command_struct(&command);
      // get_command(&command);
      continue;
    }

    if (string_compare(command.argv[0], "exit", 4) == -1)
      continue;

    if (command.memory_error_flag == true)
    {
      write(STDOUT_FILENO, "Failed to allocate more memory\n", 31);
      break;
    }

    allocate_jobs(&job, &command);
    reset_command_struct(&command);
  }

  return 0;
}
