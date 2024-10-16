#include "custom_signals.h"
#include "constants.h"
#include <unistd.h>


void handle_sigint(int signal)
{
    write(STDOUT_FILENO,"Nice try, But you can't exit that easily!\n",43);
}

