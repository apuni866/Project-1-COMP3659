#ifndef PARSE_H
#define PARSE_H

#include "constants.h"
#include "command.h"
#include "job.h"

int tokenizer(char *input_buffer, Command *command);
int parse(Command *command, Job *job);
void handle_special_token(Command *command, Job *job, int *i);
int check_token(char **tokens, int i);
void handle_pipeline(Job *job, Command *command);

void handle_IO(Job *job, Command *command, int *i, char io_char);

#endif
