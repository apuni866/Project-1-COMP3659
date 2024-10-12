#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <stdio.h> //remove this later

#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"
#include "job.h"

void run_job(Job *job, Command *command)
{
    // cant handle more than 1 pipe at this point
    if (job->num_stages > 1)
    {
        perror("Error! More than 1 pipe was detected\n");
        job->num_stages = 0;
        // make a reset job funtion as well
        return;
    }
    if (job->outfile_path != NULL)
    {
        printf("Inisde of job != outfile path\n");
        // open_output_file(job,command);
        command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG);
        if (command->output_fd == -1)
        {
            perror("Something went wrong with opening the specified file.\n");
            return;
        }
    }
    printf("jobs->outfile: %s\n", job->outfile_path);

    printf("Calling run_command...\n");
    run_command(command, command->input_fd, command->output_fd);
    reset_job(job);
}

void reset_job(Job *job)
{
    job->infile_path = NULL;
    job->outfile_path = NULL;
    int i;

    for (i = 0; i < job->num_stages; i++)
    {
        reset_command_struct(&job->pipeline[i]);
    }
    job->num_stages = 0;
}

void open_output_file(Job *job, Command *command)
{
    command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG);
    if (command->output_fd == -1)
    {
        perror("Something went wrong with opening the specified file.\n");
        return;
    }
}