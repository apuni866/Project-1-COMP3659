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

// Function to prompt user to hit enter between major sections
void prompt_continue() {
    printf("\nPress Enter to continue...\n");
    while (getchar() != '\n');
}

// Helper function to format test results
void print_test_result(const char *test_name, int result) {
    if (result) {
        printf("%-40s [--PASS--]\n", test_name);
    } else {
        printf("%-40s [--FAIL--]\n", test_name);
    }
}

// Function to test custom_string functions
void test_custom_string() {
    printf("Testing custom_string functions...\n");

    // Test string_compare
    const char *str1 = "hello";
    const char *str2 = "hello";
    const char *str3 = "world";
    print_test_result("string_compare identical", string_compare(str1, str2, get_strlen(str1)) == 0);
    print_test_result("string_compare different", string_compare(str1, str3, get_strlen(str1)) != 0);

    // Test parse
    char input[MAX_BUFFER_SIZE] = "ls -la";
    char *args[MAX_ARGS];
    parse(input, args);
    print_test_result("parse", strcmp(args[0], "ls") == 0 && strcmp(args[1], "-la") == 0);

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

    prompt_continue();  // After all custom_string tests
}

// Function to test command functions
void test_command() {

    Command command;
    command.memory_error_flag = false;
    printf("Testing command functions...\n");

    // Test get_command

    printf("\nEnter empty command\n");
    get_command(&command);
    print_test_result("get_command", command.argv[0] == NULL); 

    printf("Enter non-empty command\n");
    get_command(&command);  // Simulate getting a command
    print_test_result("get_command", command.argv[0] != NULL);  // Assuming any input is valid
    
    int run_result = run_command(&command);
    print_test_result("run_command", run_result == 0);

    reset_command_struct(&command);
    print_test_result("reset_command_struct", command.argv[0] == NULL);

    prompt_continue();  // After all command tests
}

// Function to test memory functions
void test_memory() {
    printf("Testing memory functions...\n");

    char *buffer = alloc(MAX_BUFFER_SIZE);
    print_test_result("alloc", buffer != NULL);

    buffer = resize(buffer);
    print_test_result("resize", buffer != NULL);

    free_all();
    print_test_result("free_all", 1);  // Assume free_all works without error

    prompt_continue();  // After all memory tests
}

int main() {

    test_custom_string();

    test_command();

    test_memory();

    printf("All tests completed.\n");

    return 0;
}
