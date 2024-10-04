#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
<<<<<<< HEAD


=======
>>>>>>> 653a168292677baedf66f9d0691c0a63f02bd355
  
int main ()
{
  Command command;
  command.memory_error_flag = false;
 
  while (true)
  {
    get_command(&command);

    if (string_compare(command.argv[0],"exit",4) == 0)    
      break;
    
    if (string_compare(command.argv[0],"exit",4) == -1)
      continue;    

    if (command.memory_error_flag == true)
    {
      write(STDOUT_FILENO, "Failed to allocate more memory\n",31);
      break;
    }

    run_command(&command);
    free_all();
 
  }
<<<<<<< HEAD


=======
>>>>>>> 653a168292677baedf66f9d0691c0a63f02bd355
  return 0;
}


//one child ..?
/*
int spawn_child_process()
{

  
  return 0; 

}
*/


