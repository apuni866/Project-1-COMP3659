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
 
  while (1)
  {

    get_command(&command);

    if (string_compare(command.argv[0],"exit",4) == 0)     
      break;

    if (command.memory_error_flag == true)
    {
      write(STDOUT_FILENO, "Failed to allocate more memory\n",31);
      break;
    }

    free_all();
    
 
  }
 
  /**** basic test for the get_strnlen and strncat funtions ********* 
  size_t srcLen = get_strlen(temp1);
  size_t destLen = get_strlen(temp2);
  size_t space_left = (size_t)MAX_BUFR_SZ - destLen - 1; //minus 1 as you dont count the null terminator in the get_strlen funtion  

  if (space_left > srcLen)                         
  {
    printf("Yes, there is enough size in the dest to cpy\n");
    printf("%lu\n", srcLen);
    printf("%lu\n", destLen);
    printf("%lu\n", space_left);

    printf("%s\n", temp2);    
    
  }
  else{
      printf("Copying failed, not enough space\n");
    }
  ************************************************/  

  /**** memory allocation test (1024 bytes max) *******/
  /*  char *block1 = alloc(500);

  if (block1 != NULL)
    printf("B1 Allocation successful\n");
  else
    printf("B1 Allocation error\n");



  char *block2 = alloc(500);

  if (block2 != NULL)
    printf("B2 Allocation successful\n");
  else
    printf("B2 Allocation error\n");


  free_all();

  char *block3 = alloc(1025);

  if (block3 != NULL)
    printf("B3 Allocation successful\n");
  else
    printf("B3 Allocation error\n");

  ****************************************************/

  
  
  return 0;
}



