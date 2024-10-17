#include <unistd.h>
#include <fcntl.h>

#include <stdio.h> //remove
#include <errno.h> //remove

#include <signal.h>
#include <bits/sigaction.h>


#include "custom_string.h"
#include "custom_signals.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"
#include "parse.h"


// void add_argument_to_pipeline(Job *job, int pipeline_index, int *argv_index, char *input_str, int pos);
// void start_new_pipeline_stage(Job *job, int *pipeline_index, int *argv_index, char *input_str, int *pos);
// void handle_special_char(Job *job, char *input_str, int pos, char *sp_char, bool *pipeline_done);
// void set_input_output_paths(Job *job, char *input_str, int *pos, char sp_char);

int create_job(Job *job, char input_str[MAX_BUFFER_SIZE]);
void add_to_history(const char *input_str);
void log_message(const char *message);

void print_command_history(char command_history[], int history_count);


struct sigaction sig;
char command_history[MAX_LOG_SIZE + 1];
char temp_str[MAX_BUFFER_SIZE];

int history_count = 0;
int increment = 0;




int main()
{
  Command command;
  Job job =
      {
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

  //add sigaction for quit
  

  reset_command_struct(&command);

  while (true)
  {
    char *input_str = get_command();

    if (input_str == NULL || *input_str == '\0')
      continue;


    /*
    add_to_history(input_str);
    print_command_history(command_history,history_count);

    if(string_compare(UP_ARROW, input_str, UP_ARROW_SIZE) == 0)
    {
      //printf("Hey up arrow was pressed\n");
      increment++;
      strcpy(temp_str , &command_history[history_count - increment]);
      write(STDOUT_FILENO, temp_str, get_strlen(temp_str) );
      write(STDOUT_FILENO, "\n", 2);
      continue;
    }
    */

    if (create_job(&job, input_str) == -1)
      continue;

    if (string_compare(job.pipeline[0].argv[0], "exit", 4) == 0)
    {
      free_all(); // Free the input string memory before exiting
      print_command_history(command_history,history_count);
      break;
    }

    if (string_compare(job.pipeline[0].argv[0], "clear", 5) == 0)
    {
      write(STDOUT_FILENO, "\033[H\033[J", 7); // ANSI escape codes to clear the screen
      reset_command_struct(&command);
      free_all(); // Free input string after each command
      continue;
    }


    run_job(&job);
    free_all(); // Free input string after the job is run
    reset_job(&job);
  

  }

  return 0;
}

int create_job(Job *job, char input_str[256])
{
  int pipeline_index = 0;
  int argv_index = 0;
  bool space_found = false;
  bool pipeline_done = false;
  char sp_char;
  int i = 0;
  // printf("%d\n", len);

  while (input_str[i] == ' ' && i < MAX_BUFFER_SIZE)
  {
    printf("i: %d\n", i);
    i++;
  }
  if (input_str[i] == '\0' || input_str[i] == '\n')
  {
    printf("input_str is null, but thats bad :(\n");
    return -1;
  }
  job->pipeline[pipeline_index].argv[argv_index] = &input_str[0];
  job->pipeline[pipeline_index].argc = 1;
  argv_index++;

  job->num_stages = 1;
  for (; i < MAX_BUFFER_SIZE; i++)
  {
    if (!pipeline_done)
    {

      if (space_found && !(input_str[i] == '|' ||
                           input_str[i] == '<' ||
                           input_str[i] == '>' ||
                           input_str[i] == '&'))
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
      else if (input_str[i] == '<' ||
               input_str[i] == '>' ||
               input_str[i] == '&')
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

/* didnt finsish these*/
//call from handler
void log_message(const char *message)
{
}
void add_to_history(const char *input_str)
{
  if (history_count < MAX_LOG_SIZE)
  {
    strncat(&command_history[history_count], input_str, get_strlen(input_str));
  } 
    // if (history_count < 100)
    //   strncat(&command_history[history_count+1] , " ", 1);

    history_count++;
}
void print_command_history(char command_history[], int history_count) {
    for (int i = 0; i < history_count; i++) {
        
        printf("%s\n",&command_history[history_count]);
        //write(STDOUT_FILENO, command_history[i], strlen(command_history[i]));
        //write(STDOUT_FILENO, "\n", 1);  // Print a newline after each command
    }
}
