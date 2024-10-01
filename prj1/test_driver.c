#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "custom_string.h"
#include "constants.h"
#include "command.h"
#include "memory.h"

// Forward declarations of functions to test
void get_command(Command *command);
int run_command(Command *command);
void reset_command_struct(Command *command);
void flush();

// Memory-related function prototypes
char *alloc(unsigned int size);
void free_all();
char *resize(char *old_buffer);

// Mock implementations of missing functions for command tests
void parse(char *buffer, char **argv)
{
    int index = 0;
    char *token = strtok(buffer, " ");
    while (token != NULL)
    {
        argv[index++] = token;
        token = strtok(NULL, " ");
    }
    argv[index] = NULL;
}

int main()
{
    // ===================
    // COMMAND TESTS
    // ===================
    Command test_command;

    // Test 1: Empty command input
    printf("=== Test 1: Empty Command ===\n");
    reset_command_struct(&test_command);
    char empty_input[] = "\n";
    write(STDIN_FILENO, empty_input, sizeof(empty_input));
    get_command(&test_command);
    if (test_command.argv[0] == NULL)
    {
        printf("Test 1 Passed: Command is NULL as expected.\n");
    }
    else
    {
        printf("Test 1 Failed: Command is not NULL.\n");
    }

    // Test 2: Valid command with arguments
    printf("=== Test 2: Valid Command ===\n");
    reset_command_struct(&test_command);
    char valid_input[] = "ls -la\n";
    write(STDIN_FILENO, valid_input, sizeof(valid_input));
    get_command(&test_command);
    if (strcmp(test_command.argv[0], "ls") == 0 && strcmp(test_command.argv[1], "-la") == 0)
    {
        printf("Test 2 Passed: Parsed command correctly.\n");
    }
    else
    {
        printf("Test 2 Failed: Command parsing incorrect.\n");
    }

    // Test 3: Command exceeding MAX_BUFFER_SIZE
    printf("=== Test 3: Command Exceeding Buffer ===\n");
    reset_command_struct(&test_command);
    char large_input[MAX_BUFFER_SIZE + 10];
    memset(large_input, 'A', MAX_BUFFER_SIZE + 9);
    large_input[MAX_BUFFER_SIZE + 9] = '\n';
    write(STDIN_FILENO, large_input, sizeof(large_input));
    get_command(&test_command); // Should trigger flush.
    printf("Test 3 Passed if flush triggered (check output).\n");

    // Test 4: Running a command that exists (e.g., "ls")
    printf("=== Test 4: Run Valid Command ===\n");
    reset_command_struct(&test_command);
    test_command.argv[0] = "ls";
    test_command.argv[1] = NULL;
    int status = run_command(&test_command);
    if (status == 0)
    {
        printf("Test 4 Passed: Command ran successfully.\n");
    }
    else
    {
        printf("Test 4 Failed: Command did not run successfully.\n");
    }

    // Test 5: Running a command that doesn't exist
    printf("=== Test 5: Run Invalid Command ===\n");
    reset_command_struct(&test_command);
    test_command.argv[0] = "non_existent_command";
    test_command.argv[1] = NULL;
    status = run_command(&test_command);
    if (status != 0)
    {
        printf("Test 5 Passed: Command failed as expected.\n");
    }
    else
    {
        printf("Test 5 Failed: Command should have failed but succeeded.\n");
    }

    // ===================
    // MEMORY TESTS
    // ===================

    // Test 6: Allocating memory within heap size
    printf("=== Test 6: Allocating Memory Within Heap ===\n");
    free_all();                // Reset heap before testing.
    char *block1 = alloc(100); // Allocate 100 bytes.
    if (block1 != NULL)
    {
        printf("Test 6 Passed: Memory allocated successfully.\n");
    }
    else
    {
        printf("Test 6 Failed: Memory allocation failed.\n");
    }

    // Test 7: Allocating memory exceeding heap size
    printf("=== Test 7: Allocating Memory Exceeding Heap ===\n");
    char *block2 = alloc(HEAP_SIZE + 1); // Request more memory than available.
    if (block2 == NULL)
    {
        printf("Test 7 Passed: Memory allocation failed as expected.\n");
    }
    else
    {
        printf("Test 7 Failed: Memory allocation should have failed but succeeded.\n");
    }

    // Test 8: Resizing memory to a larger block
    printf("=== Test 8: Resizing Memory Block ===\n");
    char *small_block = alloc(50); // Allocate a small block.
    strcpy(small_block, "Test data");
    char *resized_block = resize(small_block); // Resize to a larger block.
    if (resized_block != NULL && strcmp(resized_block, "Test data") == 0)
    {
        printf("Test 8 Passed: Resized memory and retained data successfully.\n");
    }
    else
    {
        printf("Test 8 Failed: Resizing failed or data was lost.\n");
    }

    // Test 9: Freeing all memory
    printf("=== Test 9: Free All Memory ===\n");
    free_all();
    char *block3 = alloc(HEAP_SIZE - 10); // Allocate most of the heap.
    if (block3 != NULL)
    {
        printf("Test 9 Passed: Memory was freed successfully, and re-allocation works.\n");
    }
    else
    {
        printf("Test 9 Failed: Memory was not freed correctly.\n");
    }

    return 0;
}
