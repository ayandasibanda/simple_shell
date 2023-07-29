#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
        char *full_path = malloc(strlen(dir) + strlen(command) + 2);
        if (full_path == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        sprintf(full_path, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    return NULL;
}

/**
 * execute_command - Execute a command using fork and exec.
 * @command: The command that will be executed.
 */
void execute_command(const char *command)
{
    char *full_path = find_command(command);

    if (full_path == NULL)
    {
        fprintf(stderr, "Command not found: %s\n", command);
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free(full_path);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process
        char *args[] = {full_path, NULL};
        if (execve(full_path, args, NULL) == -1)
        {
            // Print an error message if the command cannot be executed
            perror("shell");
            free(full_path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process
        int status;
        do
        {
            // Wait for the child process to complete
            if (waitpid(pid, &status, WUNTRACED) == -1)
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
 * display_prompt - Displays the shell prompt to the user.
 */
void display_prompt(void)
{
    printf("#cisfun$ ");
}

int main(void)
{
    char input[MAX_COMMAND_LENGTH];

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

