
/********************************************************************
 *
 *                            READ ME
 *
 * Our create_job function is very long, parsing.c currently is not implemented
 * in out shell because any time we tried to simplify the function, we would encounter
 * bugs when populating jobs. We decided to include our attemp at simplifying the function
 * below.
 ********************************************************************/
#if 0
int create_job(Job *job, char input_str[MAX_BUFFER_SIZE])
{
    int pipeline_index = 0;
    int argv_index = 0;
    bool space_found = false;
    bool pipeline_done = false;
    char sp_char;
    int i = 0;

    skip_leading_spaces(input_str, &i);
    if (input_str[i] == '\0' || input_str[i] == '\n')
    {
        return -1;
    }

    initialize_pipeline(job, input_str, &pipeline_index, &argv_index, &i);

    for (; i < MAX_BUFFER_SIZE; i++)
    {
        if (!pipeline_done)
        {
            process_arguments(job, input_str, &pipeline_index, &argv_index, &i, &space_found);
            process_special_characters(job, input_str, &pipeline_index, &argv_index, &i, &sp_char, &pipeline_done, &space_found);
        }
        else
        {
            finalize_job(job, input_str, &i, &sp_char);
        }
    }
    return 0;
}
void skip_leading_spaces(char input_str[MAX_BUFFER_SIZE], int *i)
{
    while (input_str[*i] == ' ' && *i < MAX_BUFFER_SIZE)
    {
        (*i)++;
    }
}

void initialize_pipeline(Job *job, char input_str[MAX_BUFFER_SIZE], int *pipeline_index, int *argv_index, int *i)
{
    job->pipeline[*pipeline_index].argv[*argv_index] = &input_str[*i];
    job->pipeline[*pipeline_index].argc = 1;
    (*argv_index)++;
    job->num_stages = 1;
}

void process_special_characters(Job *job, char input_str[MAX_BUFFER_SIZE], int *pipeline_index, int *argv_index, int *i, char *sp_char, bool *pipeline_done, bool *space_found)
{
    if (input_str[*i] == '|')
    {
        *space_found = false;
        input_str[*i] = '\0';
        (*pipeline_index)++;
        *argv_index = 0;
        job->num_stages++;
        do
        {
            (*i)++;
        } while (input_str[*i] == ' ');
        job->pipeline[*pipeline_index].argv[*argv_index] = &input_str[*i];
        job->pipeline[*pipeline_index].argc = 1;
        (*argv_index)++;
    }
    else if (input_str[*i] == '<' || input_str[*i] == '>' || input_str[*i] == '&')
    {
        *sp_char = input_str[*i];
        input_str[*i] = '\0';
        *pipeline_done = true;
        *space_found = false;
        job->pipeline[*pipeline_index].argv[*argv_index] = NULL;
    }
    else if (input_str[*i] == ' ')
    {
        input_str[*i] = '\0';
        *space_found = true;
    }
    else if (input_str[*i] == '\n' || input_str[*i] == '\0')
    {
        job->pipeline[*pipeline_index].argv[*argv_index] = NULL;
        *space_found = false;
        input_str[*i] = '\0';
    }
}

void process_arguments(Job *job, char input_str[MAX_BUFFER_SIZE], int *pipeline_index, int *argv_index, int *i, bool *space_found)
{
    if (*space_found && !(input_str[*i] == '|' || input_str[*i] == '<' || input_str[*i] == '>' || input_str[*i] == '&'))
    {
        *space_found = false;
        job->pipeline[*pipeline_index].argv[*argv_index] = &input_str[*i];
        job->pipeline[*pipeline_index].argc++;
        (*argv_index)++;
    }
}

void finalize_job(Job *job, char input_str[MAX_BUFFER_SIZE], int *i, char *sp_char)
{
    if (input_str[*i] == IO_IN || *sp_char == IO_IN)
    {
        for (; input_str[*i] == ' ' || input_str[*i] == IO_IN; (*i)++)
            input_str[*i] = '\0';
        *sp_char = ' ';
        job->infile_path = &input_str[*i];
    }
    else if (input_str[*i] == IO_OUT || *sp_char == IO_OUT)
    {
        for (; input_str[*i] == ' ' || input_str[*i] == IO_OUT; (*i)++)
            input_str[*i] = '\0';
        *sp_char = ' ';
        job->outfile_path = &input_str[*i];
    }
    else if (input_str[*i] == '&' || *sp_char == '&')
    {
        input_str[*i] = '\0';
        *sp_char = ' ';
        job->background = true;
    }
    if (input_str[*i] == '\n' || input_str[*i] == ' ')
        input_str[*i] = '\0';
}
#endif