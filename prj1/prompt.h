#ifndef PROMPT_H
#define PROMPT_H

#include "constants.h"
#include "memory.h"
#include "command.h"
#include "job.h"

int create_job(Job *job, char input_str[MAX_BUFFER_SIZE]);
void write_welcome_message();

#endif