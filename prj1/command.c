#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"

/*****************************************************************
 * @brief Writes the prompt sysmbol then gets an individual command
 * from the user, returns the command as a string
 *
 * @return char*
 ****************************************************************/
char *get_command()
{
  char *buffer = alloc(MAX_BUFFER_SIZE + 1);
  int bytesRead;
  write(STDOUT_FILENO, PROMPT_SYMBOL, PROMPT_SYMBOL_SIZE);

  bytesRead = read(STDIN_FILENO, buffer, MAX_BUFFER_SIZE - 1);
  if (bytesRead < 0)
  {
    free_all();
    return NULL;
  }

  else if (bytesRead == 0)
  {
    free_all();
    return '\0';
  }

  else if (bytesRead >= MAX_BUFFER_SIZE - 1)
    flush();

  buffer[bytesRead - 1] = '\0';
  return buffer;
}
/*****************************************************************
 * @brief Resets the command struct to its default values, asigning argv to
 *        NULL and argc to 0. Also frees up heap memory.
 *
 * @param command
 ****************************************************************/
void reset_command_struct(Command *command)
{
  command->argc = 0;
  free_all();

  for (int i = 0; i < MAX_ARGS; i++)
  {
    command->argv[i] = NULL;
  }
}

/***********************************
 * @brief Flushes the input buffer
 *
 **********************************/
void flush()
{
  char discard;
  while (read(STDIN_FILENO, &discard, 1) > 0 && discard != '\n')
    ;
}
