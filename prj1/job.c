#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <stdio.h>   //remove this later
 
#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"
#include "job.h"



void run_job(Job * job, Command* command)
{
    //cant handle more than 1 pipe at this point
    if (job->num_stages > 1)
    {
        perror("Error! More than 1 pipe was detected\n");
        job->num_stages = 0;
        //make a reset job funtion as well
        return;
    }
    // else if (job->num_stages == 1){
    //     int pipefd[2];
    //     int i = 0;
    // }
    // else{
    //     run_command(command);
    // }
    

    //loop for the pipes
    // for (int i = 0; i < job->num_stages; i++){
    //     ;

    // }
   
    if (job->outfile_path != NULL)
    {      
        printf("Inisde of job outfile path != NULL\n");
      //open_output_file(job,command);
      command->output_fd = open(job->outfile_path, O_CREAT | O_TRUNC | O_WRONLY , FILE_FLAG); 
      if (command->output_fd == -1){
        perror("Something went wrong with opening the specified file.\n");
        return;
     }
       
    }
    if (job->infile_path != NULL)
    {
      command->input_fd = open(job->infile_path, O_RDONLY); 
      if (command->input_fd == -1)
      {
        perror("This file does not exist.\n");
        return;

      }


    }

    print_argv(command);

    printf("command->output_fd = %d\n",command->output_fd);
    printf("command->input_fd = %d\n",command->input_fd);


    // if (job->infile_path != NULL)
    // {
    //     ;
    // }
    
    printf("Calling run_command...\n");

    //list_all_debug(command);

    run_command(command, command->input_fd, command->output_fd);
    reset_job(job);
    reset_command_struct(command);

}


void reset_job(Job *job)
{
    job->infile_path = NULL;
    job->outfile_path = NULL;
    int i;

    for (i = 0; i < job->num_stages; i++){
        reset_command_struct(&job->pipeline[i]);
    }
    job->num_stages = 0;
}

void open_output_file(Job *job,Command *command)
{
    command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG); 
    if (command->output_fd == -1){
        perror("Something went wrong with opening the specified file.\n");
        return;
    }
}

void print_argv(Command *command)
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARGUMENT VECTOR DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Command Arguments (argc = %d):\n", command->argc);

    // Check if command->argv is NULL
    if (command->argv == NULL) {
        printf("argv is NULL\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        return;
    }

    for (unsigned int i = 0; i < command->argc; i++)
    {
        if (command->argv[i] != NULL)
        {
            printf("argv[%d]: %s\n", i, command->argv[i]);
        }
        else
        {
            printf("argv[%d]: NULL\n", i);
        }
    }

    // Print the last NULL pointer in argv if applicable
    if (command->argc > 0 && command->argv[command->argc] == NULL)
    {
        printf("argv[%d]: NULL\n", command->argc);
    }
    
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END DEBUG ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void list_all_debug(Command *command)
{
  
    // Check if command or command->argv is NULL
    if (command == NULL) {
        printf("command is NULL\n");
        return;
    }

    if (command->argv == NULL) {
        printf("argv is NULL\n");
        return;
    }

    printf("Arguments:\n");
    
    // Use the argc to limit the loop iterations, if available
    for (int i = 0; i <= command->argc; i++) {
        if (command->argv[i] != NULL) {
            printf("argv[%d]: %s\n", i, command->argv[i]);
        } else {
            printf("argv[%d]: NULL\n", i);
        }
    }

    // Optionally print the last NULL pointer if it's expected to be there
    if (command->argc > 0 && command->argv[command->argc] == NULL) {
        printf("argv[%d]: NULL\n", command->argc);
    }
}