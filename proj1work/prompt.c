#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"


#define MAX_BUFR_SZ 256

int main ()
{
  char cmd_input[MAX_LEN];           // input from the command line
  char *arguments[MAX_ARGS];         /* array of ptrs */
  ssize_t  bytes_read;
  int argument_count = 0;

  char temp1[] = "hello bro";
  char temp2[50] = "this is (destin)";
  
  /*while ((bytes_read = read(STD_INPUT, cmd_input, (size_t)MAX_LEN) > 0) && strncmp(cmd_input,"exit",4) != 0)*/
  write(STDOUT_FILENO,"$ ",2);
  bytes_read = read(STD_INPUT, cmd_input, (size_t)MAX_LEN);

  while ((string_compare (cmd_input,"exit",4) != 0) && bytes_read < MAX_LEN) {
    
    write(STDOUT_FILENO, cmd_input, (size_t)bytes_read);
    write(STDOUT_FILENO,"$ ",2);
    bytes_read = read(STD_INPUT, cmd_input, (size_t)MAX_LEN);
    
    argument_count = parse(cmd_input, arguments);

    printf("---> %d\n", argument_count);
    // printf("%c\n", cmd_input[0]);    
  
  }


  //*---------------*/
  // basic test for the get_strnlen funtion 
  size_t srcLen = get_strlen(temp1);
  size_t destLen = get_strlen(temp2);
  size_t space_left = (size_t)MAX_BUFR_SZ - destLen - 1; /*minus 1 as you dont count the null terminator in the get_strlen funtion */ 
  if (space_left > srcLen)                         
  {
    printf("Yes, there is enough size in the dest to cpy\n");
    
    printf("%lu\n", srcLen);
    printf("%lu\n", destLen);
    printf("%lu\n", space_left);
  }
  /*------------------------------*/

      
  return 0;
}
