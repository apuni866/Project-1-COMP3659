#ifndef JOB_H
#define JOB_H

#include "command.h"

/* ls -al /library | wc -l > output.txt &
 */
// example of a job for the sake of understanding
// it has 2 commands
// first command store in job.pipline[0]
// seconds commands store in job.pipeline[1]
//
#define MAX_PIPELINE_LEN 10

typedef struct
{
     Command pipeline[MAX_PIPELINE_LEN];
     unsigned int num_stages;
     char *outfile_path; /* NULL for no output redirection     */
     char *infile_path;  /* NULL for no input redirection      */
     int background;     /* 0=foreground, otherwise=background */
} Job;

void run_job(Job *job);
void allocate_jobs(Job *job, Command *command);
void reset_job(Job *job);
void open_output_file(Job *job, Command *command);

void print_argv(Command *command, char *message);
void print_job(Job *job, char *message);
void printArray(char *arr[], size_t size);


#endif
