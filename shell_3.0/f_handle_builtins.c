#include "shell.h"

/**
 * handle_builtins - Executes the builtin funtions in case the command is one
 * @commands: User's input parsed as an array of commads
 *
 * Return: 1 if the command is a builtin, 0 otherwise
*/
int handle_builtins(char **commands)
{
	if (commands[0] == NULL)
		return (0);

	if (strcmp(commands[0], "env") == 0)
	{
		if (commands[1] != NULL)
			return (0);

		env();
		return (1);
	}

	if (strcmp(commands[0], "setenv") == 0)
	{
		char *err_msg;

		if (commands[1] == NULL || commands[2] == NULL || commands[3] != NULL)
		{
			err_msg = "Error: setenv command needs exactly two arguments\n";
			write(STDOUT_FILENO, err_msg, strlen(err_msg));
			return (1);
		}

		_setenv(commands[1], commands[2]);
		return (1);
	}

	if (strcmp(commands[0], "unsetenv") == 0)
	{
		char *err_msg;

		if (commands[1] == NULL || commands[2] != NULL)
		{
			err_msg = "Error: unsetenv command needs exactly one argument\n";
			write(STDOUT_FILENO, err_msg, strlen(err_msg));
			return (1);
		}

		_unsetenv(commands[1]);
		return (1);
	}

	if (strcmp(commands[0], "cd") == 0)
	{
		char *path = commands[1];

		if (commands[1] == NULL)
			path = getenv("HOME");

		if (path == NULL)
			path = "/";

		_cd(path);
		return (1);
	}

	return (0);
}
