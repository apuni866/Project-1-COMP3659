#ifndef CUSTOM_SIGNALS_H
#define CUSTOM_SIGNALS_H

//this is the signal for CTRL-C (this is for prevent our shell from being terminated completely )
void handle_sigint(int signal);       

void handle_sigquit(int signal);

void handle_sigtstp(int singal);



#endif