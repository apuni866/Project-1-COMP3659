#include "custom_string.h"
#include "constants.h"
#include "command.h"



/************************
 * This is a basic 'strncmp' funtion which
 *  compares if two string are equal. Very
 *  similar to the one from the string.h library.
 *
 *  With this, we are able to remove #include <string.h>
 *
 *
 *******************/
int string_compare(const char * string1, const char* string2, size_t byte_length)
{
  unsigned char str1,str2;
  if (string1 == '\0') return -1;
  while(byte_length-- > 0){

    str1 = (unsigned char) *string1++;
    str2 = (unsigned char) *string2++;
    
    if (str1 != str2)
      return str1 - str2;

    if (str1 == '\0')
      return 0;
  }
  return 0;
}

/************************************
 * This routine return the number of arguments present
 *    within the cmd line.
 *
 *  Ex. $ do something
 *
 *    This returns '2' (treats 'do' as 1 and 'something' as 1)
 *
 *
 *
 *
 ***********************************/
int parse (char *input_buffer, char *args[])
{
  int counter = 0;

  //ignore white space
  while(*input_buffer == ' ')
  input_buffer++;


  //this main loop fills the argument array with the arguments found from the input buffer
  while(*input_buffer != '\0' && counter < MAX_ARGS){ // -1 ??

    args[counter] = input_buffer;
    counter++;

    //move ptr along
    while(*input_buffer != ' ' && *input_buffer != '\0')
      *input_buffer++;


    //repace whitespace with a null or slash (will be helpfull later when dealing with the bin/ ...)
    if (*input_buffer == ' '){

      //*input_buffer = '\0';
      *input_buffer = '\0';
      *input_buffer++;

      }
  }
  // nake note of this null 
  args[counter] = NULL; //null terminate this for execve(x,y,NULL) as it is a requirement (says in the manual 2 execve)

  return counter;

}

/*********
 *  This is a bsaic string copy funtion which copies 
 *   a string from the source into the destintation 
 *   and returns a pointer to the destination
 *
 *
 ***********/
char *strcpy (char *destination, const char *source)
{

  int i;

  for(i = 0; source[i] != '\0'; i++){
    destination[i] = source[i];

  }

  destination[i] = '\0';


  return destination;

}
/****************
 * Basic string length counter
 *
 ***/
size_t get_strlen(const char *input_str)
{
  int i = 0;
  size_t counter = 0;

  while(input_str[i] != '\0'){

    counter++;
    i++;
  }

  return counter;
}
/*************
 * This is a basic string concat (with length bytes) funtion 
 *
 *
 *******/
char *strncat(char *destination, const char *source, size_t src_byte_len)
{
  size_t i = 0;

  
  while (*destination != '\0'){
    destination++;
  }
  
  while( i < src_byte_len && *source != '\0'){
    destination[i] = source[i];
    i++;
  }


  return destination;
  
}
/*Checks if an argument in argv contains the piping character "|"*/
int contains_pipe_char(Command *command) {
    for (unsigned int i = 0; i < command->argc; i++) {
        if (string_compare(command->argv[i], "|", get_strlen(command->argv[i])) == 0) {
            return 1;  // Pipe found
        }
    }
    return 0;  // No pipe found
}

/***************************************************
* Checks if an argument in argv contains the I/O 
* redirection characters ">" or "<"
*
* Returns 1 is ">" is found 
* Returns 2 if "<" is found
* Returns 0 if neither is found
*****************************************************/
int contains_redirection_char(Command *command) {
    for (unsigned int i = 0; i < command->argc; i++) {
      
        if (string_compare(command->argv[i], ">", get_strlen(command->argv[i])) == 0) 
            return OUT_REDIRECT_CODE;  
        
        if (string_compare(command->argv[i], "<", get_strlen(command->argv[i])) == 0) 
            return IN_REDIRECT_CODE;  
    }
    return 0;  // No redirection found
}
