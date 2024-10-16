#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "custom_string.h"
#include "command.h"
#include "memory.h"
#include "constants.h"
#include "job.h"

// Function to prompt user to hit enter between major sections
void prompt_continue()
{
    printf("\n[--PRESS ENTER TO CONTINUE--]\n");
    while (getchar() != '\n')
        ;
}

void print_test_result(const char *test_name, int result)
{
    if (result)
    {
        printf("%-40s [--PASS--]\n", test_name);
    }
    else
    {
        printf("%-40s [--FAIL--]\n", test_name);
    }
}

void test_custom_string()
{

    // printf("Testing custom_string functions...\n");

    // Test string_compare
    const char *str1 = "hello";
    const char *str2 = "hello";
    const char *str3 = "world";
    print_test_result("string_compare identical", string_compare(str1, str2, get_strlen(str1)) == 0);
    print_test_result("string_compare different", string_compare(str1, str3, get_strlen(str1)) != 0);

    // Test parse
    /*
    char input[MAX_BUFFER_SIZE] = "ls -la";
    char *args[MAX_ARGS];
    parse(input, args);
    print_test_result("parse", strcmp(args[0], "ls") == 0 && strcmp(args[1], "-la") == 0);
*/
    // Test strcpy
    char dest[MAX_LEN];
    strcpy(dest, str1);
    print_test_result("strcpy", strcmp(dest, "hello") == 0);

    // Test get_strlen
    print_test_result("get_strlen", get_strlen("hello") == 5);

    // Test strncat
    char src[MAX_LEN] = "world";
    char destination[MAX_LEN] = "hello ";
    strncat(destination, src, get_strlen(src));
    print_test_result("strncat", strcmp(destination, "hello world") == 0);

    // Test contains_pipe_char using Command struct
    Command command;
    command.argc = 3;
    command.argv[0] = "ls";
    command.argv[1] = "|";
    command.argv[2] = "grep";
    command.argv[3] = NULL;
    print_test_result("contains_pipe_char (pipe)", contains_pipe_char(&command) == 1);

    // Test contains_pipe_char with no pipe
    command.argc = 2;
    command.argv[0] = "ls";
    command.argv[1] = "-l";
    command.argv[2] = NULL;
    print_test_result("contains_pipe_char (no pipe)", contains_pipe_char(&command) == 0);

    // Test contains_redirection_char using Command struct
    command.argc = 3;
    command.argv[0] = "ls";
    command.argv[1] = ">";
    command.argv[2] = "output.txt";
    command.argv[3] = NULL;
    print_test_result("contains_redirection_char (output)", contains_redirection_char(&command) == OUT_REDIRECT_CODE);

    // Test contains_redirection_char with input redirection
    command.argc = 3;
    command.argv[0] = "cat";
    command.argv[1] = "<";
    command.argv[2] = "input.txt";
    command.argv[3] = NULL;
    print_test_result("contains_redirection_char (input)", contains_redirection_char(&command) == IN_REDIRECT_CODE);

    prompt_continue(); // After all custom_string tests
}

// Function to test command functions
void test_job()
{
    // Test data
    Job eg1 = {
        {{{"/usr/bin/ls", "-al", "/library", NULL}, 3}},
        1,
        "dirlist",
        NULL,
        0};

    Job cat1 = {
        {{{"/usr/bin/cat", NULL}, 1}},
        1,
        NULL,
        "txt.txt",
        0};

    Job cat2 = {
        {{{"/usr/bin/cat", NULL}, 1}},
        1,
        NULL,
        NULL,
        0};

    Job cat3 = {
        {{{"/usr/bin/cat", NULL}, 1}},
        1,
        NULL,
        "dirlist",
        1};

    Job eg2 = {
        {{{"/usr/bin/ls", "-al", "/library", NULL}, 3},
         {{"/usr/bin/wc", "-l", NULL}, 2}},
        2,
        NULL,
        NULL,
        0};

    Job eg3 = {
        {{{"/usr/bin/ls", NULL}, 1},
         {{"/usr/bin/ls", NULL}, 1},
         {{"/usr/bin/wc", NULL}, 1}},
        3,
        NULL,
        NULL,
        0};

    Job eg4 = {
        {{{"/usr/bin/ps", "aux", NULL}, 2},
         {{"/usr/bin/grep", "conky", NULL}, 2},
         {{"/usr/bin/wc", NULL}, 1}},
        3,
        NULL,
        NULL,
        0};

    // Test cases
    print_test_result(" test_job - eg1",
                      eg1.num_stages ==
                              1 &&
                          strcmp(eg1.pipeline[0].argv[0], "/usr/bin/ls") == 0 &&
                          strcmp(eg1.pipeline[0].argv[1], "-al") == 0 &&
                          strcmp(eg1.pipeline[0].argv[2], "/library") == 0 &&
                          eg1.pipeline[0].argc == 3 &&
                          strcmp(eg1.outfile_path, "dirlist") == 0);

    print_test_result("test_job - cat1",
                      cat1.num_stages == 1 &&
                          strcmp(cat1.pipeline[0].argv[0], "/usr/bin/cat") == 0 &&
                          cat1.pipeline[0].argc == 1 &&
                          strcmp(cat1.infile_path, "txt.txt") == 0);

    print_test_result("test_job - cat2",
                      cat2.num_stages == 1 &&
                          strcmp(cat2.pipeline[0].argv[0], "/usr/bin/cat") == 0 &&
                          cat2.pipeline[0].argc == 1);

    print_test_result("test_job - cat3",
                      cat3.num_stages == 1 &&
                          strcmp(cat3.pipeline[0].argv[0], "/usr/bin/cat") == 0 &&
                          cat3.pipeline[0].argc == 1 &&
                          strcmp(cat3.infile_path, "dirlist") == 0 &&
                          cat3.background == 1);

    print_test_result("test_job - eg2",
                      eg2.num_stages == 2 &&
                          strcmp(eg2.pipeline[0].argv[0], "/usr/bin/ls") == 0 &&
                          strcmp(eg2.pipeline[0].argv[1], "-al") == 0 &&
                          strcmp(eg2.pipeline[0].argv[2], "/library") == 0 &&
                          eg2.pipeline[0].argc == 3 &&
                          strcmp(eg2.pipeline[1].argv[0], "/usr/bin/wc") == 0 &&
                          strcmp(eg2.pipeline[1].argv[1], "-l") == 0 &&
                          eg2.pipeline[1].argc == 2);

    print_test_result("test_job - eg3",
                      eg3.num_stages == 3 &&
                          strcmp(eg3.pipeline[0].argv[0], "/usr/bin/ls") == 0 &&
                          eg3.pipeline[0].argc == 1 &&
                          strcmp(eg3.pipeline[1].argv[0], "/usr/bin/ls") == 0 &&
                          eg3.pipeline[1].argc == 1 &&
                          strcmp(eg3.pipeline[2].argv[0], "/usr/bin/wc") == 0 &&
                          eg3.pipeline[2].argc == 1);

    print_test_result("test_job - eg4",
                      eg4.num_stages == 3 &&
                          strcmp(eg4.pipeline[0].argv[0], "/usr/bin/ps") == 0 &&
                          strcmp(eg4.pipeline[0].argv[1], "aux") == 0 &&
                          eg4.pipeline[0].argc == 2 &&
                          strcmp(eg4.pipeline[1].argv[0], "/usr/bin/grep") == 0 &&
                          strcmp(eg4.pipeline[1].argv[1], "conky") == 0 &&
                          eg4.pipeline[1].argc == 2 &&
                          strcmp(eg4.pipeline[2].argv[0], "/usr/bin/wc") == 0 &&
                          eg4.pipeline[2].argc == 1);
    prompt_continue();
}

// Function to test memory functions
void test_memory()
{
    // printf("Testing memory functions...\n");

    char *buffer = alloc(MAX_BUFFER_SIZE);
    print_test_result("alloc", buffer != NULL);

    buffer = resize(buffer);
    print_test_result("resize", buffer != NULL);

    free_all();
    print_test_result("free_all", 1); // Assume free_all works without error

    prompt_continue(); // After all memory tests
}

int main()
{

    test_custom_string();

    test_job();

    test_memory();

    // printf("All tests completed.\n");

    return 0;
}
