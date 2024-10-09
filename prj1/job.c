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

    //job->num_stages = (unsigned int) contains_pipe_char(command);          //num_stages is the total pipes
    //printf("The number of pipes are: %d",job->num_stages);
    job->num_stages = contains_pipe_char(command);
    
   

    
    //cant handle more than 1 pipe at this point
    // if (job->num_stages > 1){
    //     perror("Error! More than 1 pipe was detected\n");
    //     return;
    // }


}
