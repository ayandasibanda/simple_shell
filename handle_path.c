#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * find_command - Helper function to find the full path of the command in PATH
 * @command: The command that will be searched
 *
 * Return: A pointer to the full path of the command, NULL if not found
 */
char *find_command(const char *command)
{
    char *path = getenv("PATH");
    char *dir = strtok(path, ":");
    while (dir != NULL)
    {
        char *_path = malloc(strlen(dir) + strlen(command) + 2);
        if (_path == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        sprintf(_path, "%s/%s", dir, command);
        if (access(_path, X_OK) == 0)
            return _path;
        free(_path);
        dir = strtok(NULL, ":");
    }
    return NULL;
}

/**
 * execute_command - Executes the command
 * @command: The command to be executed
 *
 * Description: This function executes the specified command.
 * It handles the PATH, checks if the command exists before forking,
 * and prints an error message if the command is not found.
 */
void execute_command(const char *command)
{
    char *full_path = find_command(command);

    if (full_path == NULL)
    {
        fprintf(stderr, "Command not found: %s\n", command);
        return;
    }

    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free(full_path);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* Child process */
        char *args[] = {full_path, NULL};
        if (execve(full_path, args, NULL) == -1)
        {
            /* Print an error message if the command cannot be executed */
            perror("shell");
            free(full_path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
        do
        {
            /* Wait for the child process to complete */
            if (waitpid(pid, NULL, WUNTRACED) == -1)
            {
                perror("waitpid");
                free(full_path);
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    free(full_path);
}

/**
 * main - Entry point for the shell program
 *
 * Return: Always returns EXIT_SUCCESS
 */
int main(void)
{
    char input[MAX_COMMAND_LENGTH];

    while (1)
    {
        /* Display shell prompt */
        printf(":) ");

        /* Read the user input (command line) */
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            /* Handle end-of-file condition (Ctrl+D) */
            printf("\n");
            break;
        }

        /* Remove the trailing newline character */
        input[strcspn(input, "\n")] = '\0';

        /* Execute the command */
        execute_command(input);
    }
    return (EXIT_SUCCESS);
}

