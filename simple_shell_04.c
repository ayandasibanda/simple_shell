#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

/**
 * display_prompt - displays the shell prompt.
 */
void display_prompt(void)
{
	printf(":) ");
}

/**
 * execute_command -  Executes a command.
 * @args:An array of strings containing the command and its arguments.
 */
void execute_command(char *args[])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("./shell");
	}
	else if (pid == 0)
	{
		/* Child process*/
		if (execvp(args[0], args) == -1)
		{
			perror("./shell");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process*/
		wait(NULL);
	}
}

/**
 * command_exists - Checks if the command exists in the given PATH directories.
 * @command: The command to check.
 * @path: The value of the PATH environment variable.
 * Return: 1 if the command is found, 0 otherwise.
 */
int command_exists(const char *command, const char *path)
{
	char command_path[MAX_INPUT_LENGTH];
	char *path_dir = strtok(path, ":");

	while (path_dir != NULL)
	{
		snprintf(command_path, sizeof(command_path), "%s/%s", path_dir, command);
		if (access(command_path, X_OK) == 0)
		{
			return (1);
		}
		path_dir = strtok(NULL, ":");
	}

	return (0);
}

/**
 * parse_input_and_execute - Parses the input and executes the command.
 * @input: The user input.
 * @path: The value of the PATH environment variable.
 */
void parse_input_and_execute(char input[], char *path)
{
	char *args[MAX_ARGS];
	char *token;
	int i = 0;

	token = strtok(input, " \t\n");
	while (token != NULL && i < MAX_ARGS - 1)
	{
		args[i] = token;
		token = strtok(NULL, " \t\n");
		i++;
	}
	args[i] = NULL;

	if (i > 0)
	{
		if (strcmp(args[0], "exit") == 0)
		{
			exit(0); /* Exit the shell on "exit" command*/
		}

		if (command_exists(args[0], path))
		{
			execute_command(args);
		}
		else
		{
			printf("./shell: %s: command not found\n", args[0]);
		}
	}
}

/**
 * main - The main function of the shell.
 * Return: Always returns 0.
 */
int main(void)
{
	char input[MAX_INPUT_LENGTH];
	char *path = getenv("PATH");

	while (1)
	{
		display_prompt();

		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			break; /* Exit the loop on Ctrl+D (EOF)*/
		}

		/* Remove the trailing newline from the input*/
		input[strcspn(input, "\n")] = '\0';

		parse_input_and_execute(input, path);
	}

	return (0);
}
