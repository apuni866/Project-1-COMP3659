#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "custom_string.h"
#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"
#include "parse.h"

void create_job(Job *job, char input_str[256]);
void reset_job2(Job *job);

int main ()
{
  Command command;
  Job job;
  command.memory_error_flag = false;
  command.input_fd = -1;
  command.output_fd = -1;
  command.argc = 0;
  job.infile_path = NULL;
  job.outfile_path = NULL;


  //currently everything from before is commended out and im just using this as a driver with hardcoded jobs

  //test for standard output change.
  Job eg1 =
  {
    { { {"/usr/bin/ls", "-al", "/library", NULL}, 3}
    },
    1,
    "dirlist",
    NULL,
    0
  };
  //test for standard input change.
  Job cat1 =
  {
    { { {"/usr/bin/cat", NULL}, 1}
    },
    1,
    NULL,
    "txt.txt",
    0
  };

  //tests to see if foreground jobs
  //use cat bc it pauses shell for long enough its noticeable.
  Job cat2 =
  {
    { { {"/usr/bin/cat", NULL}, 1}
    },
    1,
    NULL,
    NULL,
    0
  };

  //tests to see if background job works.
  //weird cat IO error if we leave to standard input.
  //maybe quirk of cat.
  Job cat3 =
  {
    { { {"/usr/bin/cat", NULL}, 1}
    },
    1,
    NULL,
    "dirlist",
    1
  };

  //tests both input and output redirection.
  Job wc1 =
  {
    { { {"/usr/bin/wc", "-l",NULL}, 2}
    },
    1,
    "txt.txt",
    "dirlist",
    0
  };

  //2 stage piping no redirects.
  Job eg2 =
  {
    { { {"/usr/bin/ls", "-al", "/library", NULL}, 3},
      { {"/usr/bin/wc", "-l",              NULL}, 2}
    },
    2,
    NULL,
    NULL,
    0
  };

  // 3 stage piping no redirects.
  Job eg3 =
  {
    { { {"/usr/bin/ls", NULL}, 1},
      { {"/usr/bin/ls", NULL}, 1},
      { {"/usr/bin/wc", NULL}, 1}
    },
    3,
    NULL,
    NULL,
    0
  };

  Job eg4 =
  {
    { { {"/usr/bin/ps", "aux", NULL}, 2},
      { {"/usr/bin/grep", "conky", NULL}, 2},
      { {"/usr/bin/wc", NULL}, 1}
    },
    3,
    NULL,
    NULL,
    0
  };
  
  char input_str[MAX_BUFFER_SIZE];
  
  read(0, input_str, MAX_BUFFER_SIZE);
  //printf("%s",input_str);

  create_job(&job, input_str);
    for (int i = 0 ; i < job.num_stages; i++){
    for(int j = 0; j < job.pipeline[i].argc; j++){
      printf("%s ", job.pipeline[i].argv[j]);
      
    }
    printf(", argc = %d\n", job.pipeline[i].argc);
  }
  printf("num stages: %d\n", job.num_stages);
  printf("outfile path: %s\n", job.outfile_path);
  printf("infile path: %s\n", job.infile_path);
  if(job.background)
    printf("its a background job! %d\n", job.background);
  else
    printf("its a foreground job! %d\n", job.background);
  

  

  run_job(&job);

  
  
  //resets input str
    for(int i ; i < MAX_BUFFER_SIZE; i++)
  input_str[i] = '\0';
  //run_job(&eg4);
  return 0;
};

void create_job(Job *job, char input_str[256]){
  int len = get_strlen(input_str);
  int pipeline_index = 0;
  int argv_index = 0;
  bool space_found = false;
  bool pipeline_done = false;
  char sp_char;
  //printf("%d\n", len);

  job->pipeline[pipeline_index].argv[argv_index] = &input_str[0];
  job->pipeline[pipeline_index].argc = 1;
  argv_index++;

  job->num_stages = 1;
  for (int i = 0; i < MAX_BUFFER_SIZE ; i++){
    if(!pipeline_done){

      
      if(space_found&& !(input_str[i] == '|' ||
			 input_str[i] == '<' ||
			 input_str[i] == '>' ||
			 input_str[i] == '&')){
	space_found = false;
	job->pipeline[pipeline_index].argv[argv_index] = &input_str[i];
	job->pipeline[pipeline_index].argc++;
	argv_index++;
      }
      if(input_str[i] == '|'){
	space_found = false;
	input_str[i] = '\0';
	pipeline_index++;
	argv_index = 0;

	job->num_stages++;
        do{
	  i++;
	}while(input_str[i] == ' ');
	job->pipeline[pipeline_index].argv[argv_index] = &input_str[i];
	job->pipeline[pipeline_index].argc = 1;
	argv_index++;	  
      }
      else if(input_str[i] == '<' ||
	      input_str[i] == '>' ||
	      input_str[i] == '&'){
	sp_char = input_str[i];
	input_str[i] = '\0';
	pipeline_done = true;
	space_found = false;
	job->pipeline[pipeline_index].argv[argv_index] = NULL;
      }
      else if (input_str[i] == ' ' ){
	
	input_str[i] = '\0';
	space_found = true;
       
      }
      else if(input_str[i] == '\n'){
	job->pipeline[pipeline_index].argv[argv_index] = NULL;
	space_found = false;
	input_str[i] = '\0';
	break;
      }
      
    }
    else{
      if(input_str[i] == IO_IN || sp_char == IO_IN){
	for(;input_str[i] == ' '|| input_str[i] == IO_IN; i++)
	  input_str[i] = '\0';
	sp_char = ' ';
	job->infile_path = &input_str[i];
	
      }
      else if(input_str[i] == IO_OUT || sp_char == IO_OUT){
	for(;input_str[i] == ' '|| input_str[i] == IO_OUT; i++)
	  input_str[i] = '\0';
	sp_char = ' ';
	job->outfile_path = &input_str[i];
	
	
      }
      else if(input_str[i] == '&' || sp_char == '&'){
	input_str[i] = '\0';
	sp_char = ' ';
	job->background = true;
	
	
      }
      if(input_str[i] == '\n' || input_str[i] == ' ')
	input_str[i] = '\0';
    }

  }
  return ;
}

void reset_job2(Job *job){
  for (int i = 0; i < MAX_PIPELINE_LEN ; i++){
    for (int j = 0; j < MAX_ARGS ; j++){
      job->pipeline[i].argv[j] = '/0';
    }
    job->pipeline[i].argc = 0;
  }
  job->num_stages = 0;
  job->infile_path = NULL;
  job->outfile_path = NULL;
  job->background = 0;


  return;
}
  
  /*
  while (true)
  {
    get_command(&command);
    parse(&command,&job);
    
    printf("IN main loop under the parse call\n");

    if (string_compare(command.argv[0],"exit",4) == 0)    
      break;

    // clear command works 
    if (string_compare(command.argv[0],"clear",5) == 0){
      write(STDOUT_FILENO, "\033[H\033[J",7);
      get_command(&command);
    }
    
    if (string_compare(command.argv[0],"exit",4) == -1)
      continue;    

    if (command.memory_error_flag == true)
    {
      write(STDOUT_FILENO, "Failed to allocate more memory\n",31);
      break;
    }

<<<<<<< HEAD
    run_command(&command);
=======
//    printf("Before RUN JOB in main\n");
//    run_job(&job,&command);
//    //run_command(&command);
>>>>>>> ea3fb28adf9445776dcabe27272b7c88482ef24f
    reset_command_struct(&command);
    free_all();
 
  }

  printf("Have bokren from the main loop\n");
  
  return 0;
}
  */


