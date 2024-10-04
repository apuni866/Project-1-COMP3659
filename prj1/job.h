#ifndef JOB_H
#define JOB_H

#include "command.h"

#define MAX_PIPELINE_LEN 2

typedef struct
{
     Command pipeline[MAX_PIPELINE_LEN];
     unsigned int num_stages;
     char *outfile_path;          /* NULL for no output redirection     */
     char *infile_path;           /* NULL for no input redirection      */
     int background;              /* 0=foreground, otherwise=background */
}Job;


#endif
