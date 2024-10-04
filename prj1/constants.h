#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_BUFFER_SIZE 256 
#define MAX_LEN 128
#define MAX_ARGS 64
#define STD_INPUT_FD 0
#define PROMPT_SYMBOL ">_> "
#define PROMPT_SYMBOL_SIZE 4
#define EXIT_SUCCESS 0
#define EXIT_FAIL 1
#define OUT_REDIRECT_CODE 1
#define IN_REDIRECT_CODE 2

typedef enum {
  false = 0,
  true = 1
} bool;


#endif
