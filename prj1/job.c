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
    //    ;

    // }
    printf("before the outfile check in job.c \n");
    if (job->outfile_path != NULL)
    {
        printf("There was a command with '>' in the terminal\n");
        command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC); 
        if (command->output_fd == -1){
            perror("Something went wrong with opening the specified file.\n");
            return;
        }
    }

    run_command(command, command->input_fd, command->output_fd);
    //run_command(command);

}
