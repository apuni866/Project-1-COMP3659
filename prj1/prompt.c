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
 
  while (true)
  {
    get_command(&command);
    printf("calling parse from main\n");
    parse(&command,&job);
   
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

    //run_job(&job,&command);
    printf("before run_command\n");
    run_command(&command);
    reset_command_struct(&command);
    free_all();
 
  }


  return 0;
}


//one child ..?
/*
int spawn_child_process()
{

  
  return 0; 

}
*/


