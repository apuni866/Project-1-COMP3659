#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"

int main ()
{
  char cmd_input[MAX_LEN];           // input from the command line
  char *arguments[MAX_ARGS];         /* array of ptrs */
  ssize_t  bytes_read;
  int argument_count = 0;
  struct Command command;
  /*while ((bytes_read = read(STD_INPUT, cmd_input, (size_t)MAX_LEN) > 0) && strncmp(cmd_input,"exit",4) != 0)*/
  write(STDOUT_FILENO,"$ ",2);
  bytes_read = read(STD_INPUT_FD, command.argv, (size_t)MAX_LEN);

  while ((string_compare (command.argv, "exit",4) != 0) && bytes_read < MAX_LEN) {
    
    /*    write(STDOUT_FILENO, cmd_input, (size_t)bytes_read);
    write(STDOUT_FILENO,">_> ",4);
    bytes_read = read(STD_INPUT_FD, cmd_input, (size_t)MAX_LEN);
    
    argument_count = parse(cmd_input, arguments);

    printf("---> %d\n", argument_count);
    */
    
    get_command(&command);
  
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
