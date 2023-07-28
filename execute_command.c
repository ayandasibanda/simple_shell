#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

/**
 * display_prompt - Displays the shell prompt to the user.
 */
void display_prompt(void)
{
    printf("#cisfun$ ");
}

/**
 * execute_command - Execute a command using fork and exec.
 * @command: The command that will be executed.
 */
void execute_command(const char *command)
{
    char *args[MAX_INPUT_LENGTH];
    char *token;
    int i = 0;

    token = strtok((char *)command, " ");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("./shell");
    }
    else if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("./shell");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(NULL);
    }
}

/**
 * main - The entry point of the shell.
 *
 * Return: Always 0.
 */
int main(void)
{
    char input[MAX_INPUT_LENGTH];

    while (1)
    {
        display_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        execute_command(input);
    }

    return (0);
}

