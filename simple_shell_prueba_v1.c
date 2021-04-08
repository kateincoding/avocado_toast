#include "shell.h"

void execute_commands(char *buff, int read, char *first_av);
void execute_handling_semicolon(char *buff, int read, char *first_av);

/**
 * main - Entry point
 * @ac: number of arguments
 * @av: Array of arguments
 *
 * Return: 0 on success
*/
int main(int __attribute__((unused))ac, char **av)
{
	int read;
	char *buff = NULL;
	size_t buff_len = 0;

	while (1)
	{
		/* Print console symbol only if it is interactive*/
		if (isatty(STDIN_FILENO) == 1)
			write(STDOUT_FILENO, "#cisfun$ ", 9);
		/* Read commands from console */
		read = getline(&buff, &buff_len, stdin);
		/* Remove comments & '\n' char from buffer */
		buff = handle_comment(buff);
		_strtok(buff, "\n");
		printf("buff before handling semicolon = '%s'\n", buff);
		execute_handling_semicolon(buff, read, av[0]);
	}
	/* Free buffer memory */
	free(buff);
	return (0);
}

void execute_handling_semicolon(char *buff, int read, char *first_av)
{
	char *block = _strtok(buff, ";");

	if (block[0] == '\n')
	{
		puts(" retorna salto de linea");
		return;
	}
	while (block != NULL)
	{
		char *tmp = strdup(block);

		execute_commands(tmp, read, first_av);
		printf("block = '%s'\n", block);
		block = _strtok(NULL, ";");
		printf("block after strtok = '%s'\n", block);
		/* free (tmp); */
	}
}

void execute_commands(char *buff, int read, char *first_av)
{
	int child_pid;
	char **commands;

	/* Generate array of commands */
	commands = parse_user_input(buff);
	if (read == EOF)
	{
		free(buff);
		free_dbl_ptr(commands);
		exit(0);
	}

	/* Exit error, ENTER, and builtins */
	if (handle_exit(buff, commands) == -1 ||
	handle_enter(commands) == 1	||
	handle_builtins(commands) == 1)
	{
		free_dbl_ptr(commands);
	}

	/* Fork parent process to execute the command */
	child_pid = fork();
	if (child_pid == -1)
	{
		dispatch_error(first_av, 1);
	}
	else if (child_pid == 0)
	{
		/* Search command using the PATH env variable */
		handle_PATH(commands);
		/* execute command */
		execve(commands[0], commands, NULL);
		/* free memory */

		free(buff);

		free_dbl_ptr(commands);
		/* handle errors */
		dispatch_error(first_av, 1);
	}
	else
		wait(NULL);
	free_dbl_ptr(commands);
}

