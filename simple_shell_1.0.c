#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

void display_prompt(void);
void execute_command(char *args[]);
void print_environment(void);
int is_builtin_command(const char *command);
void execute_builtin_command(char *command);
void execute_external_command(char *args[], char *path);
void parse_input_and_execute(char *input, char *path);
/**
 * display_prompt - displays a prompt
 *
 */
void display_prompt(void)
{
	printf("$ ");
}

/**
 * execute_command - will excute a command
 * @args: array of commands
 *
 */
void execute_command(char *args[])
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("./simple_shell");
	}
	else if (pid == 0)
	{
		/* Child process */
		if (execvp(args[0], args) == -1)
		{
			perror("./simple_shell");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process */
		wait(NULL);
	}
}

/**
 * print_environment - will display an environment
 *
 */
void print_environment(void)
{
	 char **environ;
	char **env = environ;

	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * is_builtin_command - checks for builtin commands
 * @command: command to execute
 *
 * Return: 1 on success, -1 otherwise
 */
int is_builtin_command(const char *command)
{
	return (strcmp(command, "exit") == 0 || strcmp(command, "env") == 0);
}

/**
 * execute_builtin_command - will execute builtin commands
 *
 * @command: command to execute
 *
 */
void execute_builtin_command(char *command)
{
	if (strcmp(command, "exit") == 0)
	{
		exit(0);
	}
	else if (strcmp(command, "env") == 0)
	{
		print_environment();
	}
}

/**
 * execute_external_command - for external commands
 *
 * @args: array of commands
 * @path: the path for commands
 *
 */
void execute_external_command(char *args[], char *path)
{
	char command_path[MAX_INPUT_LENGTH];
	int found = 0;
	char *path_dir = strtok(path, ":");

	while (path_dir != NULL)
	{
		snprintf(command_path, sizeof(command_path), "%s/%s", path_dir, args[0]);
		if (access(command_path, X_OK) == 0)
		{
			found = 1;
			break;
		}
		path_dir = strtok(NULL, ":");
	}

	if (found)
	{
		execute_command(args);
	}
	else
	{
		printf("./simple_shell: %s: command not found\n", args[0]);
	}
}
/**
 * parse_input_and_execute - accepts and executes input
 *
 * @input: input to check and execute
 * @path:The path of commands
 *
 */
void parse_input_and_execute(char *input, char *path)
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
		if (is_builtin_command(args[0]))
		{
			execute_builtin_command(args[0]);
		}
		else
		{
			execute_external_command(args, path);
		}
	}
}
/**
 * main - The main function
 *
 * Return: 1 on success, otherwise on failure
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
			break;
		}

		input[strcspn(input, "\n")] = '\0';
		parse_input_and_execute(input, path);
	}

	return (0);
}

