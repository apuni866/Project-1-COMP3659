#include "custom_signals.h"
#include "constants.h"
#include <unistd.h>
#include <signal.h>
#include "job.h"

extern int foreground_pid;

void handle_sigint(int signal)
{
    write(STDOUT_FILENO, "\nNice try, But you can't exit that easily!\n", 44);
}


void handle_sigquit(int signal)
{

    write(STDOUT_FILENO, "Force quit was intiaited\n",26);
    
    /* NOTE: We were trying to create a log file with the history of the shell life 
       with time stamps but didnt have enough time to finish the implementation */
    
    //write(STDOUT_FILENO, "Creating a log file for saved history...\n",42);

}

void handle_sigtstp(int singal)
{

    write(STDOUT_FILENO,"Control-Z was pressed\n",23);

    // if (foreground_pid > 0)
    // {   
    //     write(1,"inside of foreg", 16);

    //     kill(foreground_pid, SIGTSTP);  //send a signal 
    // }


}