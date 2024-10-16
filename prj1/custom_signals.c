#include "custom_signals.h"
#include "constants.h"
#include <unistd.h>


void handle_sigint(int signal)
{
    write(STDOUT_FILENO, "\nNice try, But you can't exit that easily!\n", 44);
}


void handle_sigquit(int signal)
{

    write(STDOUT_FILENO, "Force quit was intiaited\n",26);
    write(STDOUT_FILENO, "Creating a log file for saved history...\n",42);

}
