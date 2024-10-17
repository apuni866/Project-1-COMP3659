#ifndef JOB_H
#define JOB_H

#include "command.h"

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
char *construct_cmd_path(const char *command, const char *default_dir_path);

void print_job(Job *job, char *message);

#endif
