#include <stddef.h>
#include "memory.h"
#include "constants.h"
#include "custom_string.h"

#include <stdio.h>

char heap[HEAP_SIZE];
char *freep = heap;



/******
 *
 *
 *****/
char *alloc(unsigned int size)
{

  int space_left = (heap + HEAP_SIZE) - freep;

  if (space_left >= size)
  {
    char *allocated_block = freep;
    freep += size;
    return allocated_block;
  }
  
  else
    return NULL;
  

}

void free_all()
{
  freep = heap;

}


char *resize(char *old_buffer)
{
  int i;
  int old_size = get_strlen(old_buffer);
  int new_size = old_size * 2;
  char *new_block = alloc (new_size);

  printf("old buf size --> %d\n",old_size);
  printf("new buf size --> %d\n",new_size);
  
  

  // NO more memory left so return NULL
  if (new_block == NULL){

    return NULL;
  }

  // Move elements to the new block
  for (i = 0; i < old_size; i++)
    new_block[i] = old_buffer[i];

  new_block[i] = '\0';

  
  return new_block;


}
