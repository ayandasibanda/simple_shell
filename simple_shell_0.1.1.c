#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024
/**
 * my_getline - function to accept user input
 *Return:nothing
 */
char *my_getline(void)
{
	 char buffer[MAX_INPUT_LENGTH];
	 size_t current_index = 0;
	 size_t buffer_length = 0;
	char *line = NULL;
	size_t line_length = 0;

	while (1)
		{
		if (current_index >= buffer_length)
		{
			buffer_length = read(STDIN_FILENO, buffer, MAX_INPUT_LENGTH);

			if (buffer_length == 0)
			{
				return (NULL);
			} else if (buffer_length < 0)
			{
				perror("my_getline");
				return (NULL);
			}
		}

		char c = buffer[current_index++];

		if (c == '\n' || c == '\0')
		{
			line_length = current_index;
			line = (char *)malloc(line_length);
			if (line == NULL)/* checks if line variable is null*/
				{
				perror("my_getline");
				exit(EXIT_FAILURE);
				}
			strncpy(line, buffer, line_length);
			line[line_length - 1] = '\0';
			break;
		}
	}
	return (line);
}
/**
 * display_prompt - displays a prompt
 *
 */
void display_prompt(void)
{
	printf(":)\n");
}
/**
 * main - the main function
 *Return: nothing
 */
int main(void)
{
	char *input;

	while (1)
	{
		display_prompt();

		input = my_getline();

		if (input == NULL)/* checks if input that is parsed is null*/
			{
			break;
			}

		if (strcmp(input, "exit") == 0) /*if the exit command is parsed*/
			{
			free(input);
			break; /* Exit the shell on "exit" command*/
			}

		free(input);
	}

	return (0);
}

