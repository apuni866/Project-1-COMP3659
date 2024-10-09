#ifndef PARSE_H
#define PARSE_H


#include "constants.h"
#include "command.h"
#include "job.h"

int tokenizer (char *input_buffer, Command* command);

void parse(Command *command, Job *job);





#endif
