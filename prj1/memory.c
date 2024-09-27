#include <stddef.h>
#include "memory.h"


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

/*****
 *
 *
 ****/
void free_all()
{
  freep = heap;


}


