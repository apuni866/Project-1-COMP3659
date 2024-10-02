#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
  
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
  return 0;
}



