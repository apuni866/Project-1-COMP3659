#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>


/********************************** 
We know why this should not be included but for some reason signal.h was not incluing the 
sigaction struct for signal handling and compiler kept giving incomplete def'n errors 
Reasons to not include it: Portability and Safelty */
#include <bits/sigaction.h>    
/*******************************************/

#include "custom_string.h"
#include "custom_signals.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"

int create_job(Job *job, char input_str[MAX_BUFFER_SIZE]);
void write_welcome_message();
bool is_special_char(char c);
void skip_leading_spaces(const char input_str[MAX_BUFFER_SIZE], int *i);

struct sigaction sig;

int main()
{
  Command command;
  Job job = {
      {{{NULL}, 0}},
      1,
      NULL,
      NULL,
      0};
  
  sig.sa_handler = &handle_sigint;
  sig.sa_flags = 0;
  sigaction(SIGINT, &sig ,NULL);

  sig.sa_handler = &handle_sigquit;
  sigaction(SIGQUIT,&sig,NULL);
 
  sig.sa_handler = &handle_sigtstp;
  sig.sa_flags = 0;//0x10000000; //SA_RESTART
  sigaction(SIGTSTP, &sig, NULL);
  

  reset_command_struct(&command);

  write_welcome_message();
  while (true)
  {
    char *input_str = get_command();

    if (input_str == NULL || *input_str == '\0')
      continue;

    if (create_job(&job, input_str) == -1)
      continue;

    if (string_compare(job.pipeline[0].argv[0], "exit", 4) == 0)
    {
      free_all();
      break;
    }

    if (string_compare(job.pipeline[0].argv[0], "clear", 5) == 0)
    {
      write(STDOUT_FILENO, "\033[H\033[J", 7);
      reset_command_struct(&command);
      free_all();
      continue;
    }
    run_job(&job);

    free_all();
    reset_job(&job);
  }
  return 0;
}
/**
 * @brief Checks if a given character is a special character.
 *
 * This function determines whether the input character `c` is a special character.
 * Special characters are typically non-alphanumeric characters, but the exact
 * definition may vary depending on the implementation.
 *
 * @param c The character to be checked.
 * @return true if the character is a special character, false otherwise.
 */
bool is_special_char(char c)
{
  return (c == '|' || c == '<' || c == '>' || c == '&');
}
/**
 * @brief Skips leading spaces in the given input string.
 *
 * This function iterates through the input string starting from the beginning
 * and increments the index pointer until a non-space character is encountered.
 *
 * @param input_str The input string from which leading spaces are to be skipped.
 *                  It is assumed to be null-terminated and its maximum size is
 *                  defined by MAX_BUFFER_SIZE.
 * @param i Pointer to an integer that will be updated to the index of the first
 *          non-space character in the input string.
 */
void skip_leading_spaces(const char input_str[MAX_BUFFER_SIZE], int *i)
{
  while (input_str[*i] == ' ' && *i < MAX_BUFFER_SIZE)
  {
    (*i)++;
  }
}
/*****************************************************************
 * @brief Parses the input string and populates an empty job struct
 *        accordingly.
 *
 * @param job Job*
 * @param input_str char[]
 * @return int
 ****************************************************************/
int create_job(Job *job, char input_str[MAX_BUFFER_SIZE])
{
  int pipeline_index = 0;
  int argv_index = 0;
  bool space_found = false;
  bool pipeline_done = false;
  char sp_char;
  int i = 0;

  skip_leading_spaces(input_str, &i);
  if (input_str[i] == '\0' || input_str[i] == '\n')
    return -1;

  job->pipeline[pipeline_index].argv[argv_index] = &input_str[0];
  job->pipeline[pipeline_index].argc = 1;
  argv_index++;

  job->num_stages = 1;

  for (; i < MAX_BUFFER_SIZE; i++)
  {
    if (!pipeline_done)
    {
      if (space_found && !is_special_char(input_str[i]))
      {
        space_found = false;
        job->pipeline[pipeline_index].argv[argv_index] = &input_str[i];
        job->pipeline[pipeline_index].argc++;
        argv_index++;
      }
      if (input_str[i] == '|')
      {
        space_found = false;
        input_str[i] = '\0';
        pipeline_index++;
        argv_index = 0;

        job->num_stages++;
        do
        {
          i++;
        } while (input_str[i] == ' ');
        job->pipeline[pipeline_index].argv[argv_index] = &input_str[i];
        job->pipeline[pipeline_index].argc = 1;
        argv_index++;
      }
      else if (is_special_char(input_str[i]))
      {
        sp_char = input_str[i];
        input_str[i] = '\0';
        pipeline_done = true;
        space_found = false;
        job->pipeline[pipeline_index].argv[argv_index] = NULL;
      }
      else if (input_str[i] == ' ')
      {
        input_str[i] = '\0';
        space_found = true;
      }
      else if (input_str[i] == '\n' || input_str[i] == '\0')
      {
        job->pipeline[pipeline_index].argv[argv_index] = NULL;
        space_found = false;
        input_str[i] = '\0';
        break;
      }
    }
    else
    {
      if (input_str[i] == IO_IN || sp_char == IO_IN)
      {
        for (; input_str[i] == ' ' || input_str[i] == IO_IN; i++)
          input_str[i] = '\0';
        sp_char = ' ';
        job->infile_path = &input_str[i];
      }
      else if (input_str[i] == IO_OUT || sp_char == IO_OUT)
      {
        for (; input_str[i] == ' ' || input_str[i] == IO_OUT; i++)
          input_str[i] = '\0';
        sp_char = ' ';
        job->outfile_path = &input_str[i];
      }
      else if (input_str[i] == '&' || sp_char == '&')
      {
        input_str[i] = '\0';
        sp_char = ' ';
        job->background = true;
      }
      if (input_str[i] == '\n' || input_str[i] == ' ')
        input_str[i] = '\0';
    }
  }
  return 0;
}
/**
 * @brief Writes a welcome message to the standard output.
 *
 * This function outputs a predefined welcome message to the console.
 * It does not take any parameters and does not return any values.
 */
void write_welcome_message()
{
  const char *message =
      "\n********************************************\n"
      "*                                          *\n"
      "*                 Welcome!                 *\n"
      "*                                          *\n"
      "********************************************\n"
      "*               Created by:                *\n"
      "*  Amtoj Punia, Henry Nguyen, Evan Wushke  *\n"
      "********************************************\n";

  write(STDOUT_FILENO, message, get_strlen(message));
}
