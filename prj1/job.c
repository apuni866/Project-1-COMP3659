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

#define READ_END 0
#define WRITE_END 1


void run_job(Job * job)
{
  int pipefd[2] ={1,0};
  int child_status;
  int pid,pid2;
  char* const envp[] = {NULL};
  int outfile = 1;
  int infile = 0;
  int stage = 0;
  
   
  if(job->infile_path != NULL){
    infile = open(job->infile_path,O_RDONLY);
    printf("infile: %s: %d\n",job->infile_path, infile);
    dup2(infile,0);
    close(infile);
  }
  
  for(;stage < (job->num_stages-1);stage++){
    write(1, "loop\n", 6);
    pipe(pipefd);
    
    pid = fork();
    
    if (pid == 0){
      if (infile !=0){
	dup2(infile,0);
	close(infile);
      }
      
      close(pipefd[READ_END]);
      dup2(pipefd[WRITE_END],1);
      close(pipefd[WRITE_END]);
      
      execve(job->pipeline[stage].argv[0],job->pipeline[stage].argv,envp);
      
      exit(EXIT_SUCCESS);//safetey exit. replace with error trap instead later.
      
    }
    
    close(pipefd[WRITE_END]);
    infile = pipefd[READ_END];
  }
  
  
  pid2 = fork();
  if(pid2 == 0){  
     
    if(job->outfile_path != NULL){
      outfile = open(job->outfile_path,O_WRONLY | O_CREAT|O_TRUNC,0644);
      printf("outfile: %s: %d\n",job->outfile_path, outfile);
      dup2(outfile,1);
      close(outfile);
    }
    
    if (infile != 0)
      dup2(infile,0);
    if (outfile != 1){
	//dup2(pipefd[READ_END],0);
      }
	
    execve(job->pipeline[stage].argv[0],job->pipeline[stage].argv,envp);
  }
  close(pipefd[WRITE_END]);
  
  close(pipefd[READ_END]);
  if(!job->background)
    waitpid(pid2,&child_status,0);
  else
    printf("hey its a backgroundjob");
      

  /*
  pipe(pipefd);
  
  pid = fork();

  if (pid == 0){

    
    close(pipefd[READ_END]);
    dup2(pipefd[WRITE_END],1);
    
    execve(job->pipeline[stage].argv[0],job->pipeline[stage].argv,envp);

    //    exit(EXIT_SUCCESS);//safetey exit. replace with error trap instead later.
    
  }
  stage++;
  //close(pipefd[WRITE_END]);
  for (;stage < (job->num_stages-1); stage++){
	//printf("stage: %d\n", stage);
  
  };

  
  pid = fork();
  if (pid == 0){

 
    close(pipefd[WRITE_END]);
    dup2(pipefd[READ_END],0);

    execve(job->pipeline[stage].argv[0],job->pipeline[stage].argv,envp);
  }


  close(pipefd[WRITE_END]);

  close(pipefd[READ_END]);


  if(!job->background)
    waitpid(pid,&child_status,0);
  else
    printf("hey its a backgroundjob");
  */  
  
  /*
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
        //open_output_file(job,command);
        command->output_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_FLAG); 
        if (command->output_fd == -1)
        {
            perror("Something went wrong with opening the specified file.\n");
            return;
        }
    }
  

    // if (job->infile_path != NULL)
    // {
    //     ;
    // }
    
    printf("Calling run_command...\n");
    run_command(command, command->input_fd, command->output_fd);
    reset_job(job);
  */
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
