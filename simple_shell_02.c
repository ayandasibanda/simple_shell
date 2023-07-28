#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LEN 1024

/**
 * parse_command - Tokenize the command string into arguments.
 * @command: The command string to parse.
 * @args: An array to store the parsed arguments.
 *
 * Return: The number of arguments parsed.
 */
int parse_command(char command[], char *args[])
{
    int arg_count = 0;
    char *token = strtok(command, " ");

    while (token != NULL)
    {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    return arg_count;
}

/**
 * main - Simple UNIX command line interpreter (shell)
 *
 * Return: Always returns 0 to indicate successful execution.
 */
int main(void)
{
    char command[MAX_COMMAND_LEN];

    while (1)
    {
        printf("#cisfun$ ");

        if (fgets(command, MAX_COMMAND_LEN, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        char *args[MAX_COMMAND_LEN];
        int arg_count = parse_command(command, args);

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
            if (execvp(args[0], args) == -1)
            {
                perror(args[0]);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

