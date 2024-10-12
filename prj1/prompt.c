#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"
#include "parse.h"
  
int main ()
{
  Command command;
  Job job;
  command.memory_error_flag = false;
  command.input_fd = -1;
  command.output_fd = -1;
  command.argc = 0;
  job.infile_path = NULL;
  job.outfile_path = NULL;


  //currently everything from before is commended out and im just using this as a driver with hardcoded jobs

  //test for standard output change.
  Job eg1 =
  {
    { { {"/usr/bin/ls", "-al", "/library", NULL}, 3}
    },
    1,
    "dirlist",
    NULL,
    0
  };
  //test for standard input change.
  Job cat1 =
  {
    { { {"/usr/bin/cat", NULL}, 1}
    },
    1,
    NULL,
    "txt.txt",
    0
  };

  //tests to see if foreground jobs
  //use cat bc it pauses shell for long enough its noticeable.
  Job cat2 =
  {
    { { {"/usr/bin/cat", NULL}, 1}
    },
    1,
    NULL,
    NULL,
    0
  };

  //tests to see if background job works.
  //weird cat IO error if we leave to standard input.
  //maybe quirk of cat.
  Job cat3 =
  {
    { { {"/usr/bin/cat", NULL}, 1}
    },
    1,
    NULL,
    "dirlist",
    1
  };

  //tests both input and output redirection.
  Job wc1 =
  {
    { { {"/usr/bin/wc", "-l",NULL}, 2}
    },
    1,
    "txt.txt",
    "dirlist",
    0
  };

  //2 stage piping no redirects.
  Job eg2 =
  {
    { { {"/usr/bin/ls", "-al", "/library", NULL}, 3},
      { {"/usr/bin/wc", "-l",              NULL}, 2}
    },
    2,
    NULL,
    NULL,
    0
  };

  // 3 stage piping no redirects.
  Job eg3 =
  {
    { { {"/usr/bin/ls", NULL}, 1},
      { {"/usr/bin/ls", NULL}, 1},
      { {"/usr/bin/wc", NULL}, 1}
    },
    3,
    NULL,
    NULL,
    0
  };

  Job eg4 =
  {
    { { {"/usr/bin/ps", "aux", NULL}, 2},
      { {"/usr/bin/grep", "conky", NULL}, 2},
      { {"/usr/bin/wc", NULL}, 1}
    },
    3,
    NULL,
    NULL,
    0
  };
  
  
 
  run_job(&eg4);
  return 0;
};
  
  /*
  while (true)
  {
    get_command(&command);
    parse(&command,&job);
    
    printf("IN main loop under the parse call\n");

    if (string_compare(command.argv[0],"exit",4) == 0)    
      break;

    // clear command works 
    if (string_compare(command.argv[0],"clear",5) == 0){
      write(STDOUT_FILENO, "\033[H\033[J",7);
      get_command(&command);
    }
    
    if (string_compare(command.argv[0],"exit",4) == -1)
      continue;    

    if (command.memory_error_flag == true)
    {
      write(STDOUT_FILENO, "Failed to allocate more memory\n",31);
      break;
    }

<<<<<<< HEAD
    run_command(&command);
=======
//    printf("Before RUN JOB in main\n");
//    run_job(&job,&command);
//    //run_command(&command);
>>>>>>> ea3fb28adf9445776dcabe27272b7c88482ef24f
    reset_command_struct(&command);
    free_all();
 
  }

  printf("Have bokren from the main loop\n");
  
  return 0;
}
  */


