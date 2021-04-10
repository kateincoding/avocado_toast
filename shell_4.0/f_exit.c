#include "shell.h"

/**
 * handle_exit - Checks if the user entered the exit command
 * @buff: User's input
 * @commands: User's input parsed as array of commands
 *
 * Return: 0 if the commad is NOT exit, -1 if the exit status was Illegal
*/
int handle_exit(char *buff, char **commands)
{
	int status;

	/* Command is NOT exit */
	if (commands[0] == NULL || strcmp(commands[0], "exit") != 0)
		return (0);

	/* Command is exit */
	if (commands[1] == NULL)
	{
		free(buff);
		free_dbl_ptr(commands);
		exit(0);
	}

	status = get_exit_status(commands[1]);
	/* Command is exit status */
	if (status >= 0)
	{
		free(buff);
		free_dbl_ptr(commands);
		exit(status);
	}

	/* the exit status passed was illegal */
	print_Illegal_exit_status(commands[1]);
	return (-1);
}

/**
 * get_exit_status - Calculates the exit status as a number
 * @buff: User's input
 *
 * Return: Exist status as number, -1 on error
*/
int get_exit_status(char *buff)
{
	int i;
	int status = 0;

	for (i = 0; buff[i] != '\0'; i++)
	{
		if (buff[i] == '\n')
			return (status);

		if (buff[i] < '0' || buff[i] > '9')
			return (-1);

		status *= 10;
		status += buff[i] - '0';
	}

	return (status);
}

/**
 * print_Illegal_exit_status - Prints the error message to the screen
 * @status_str: Staus passed as exit status argument
*/
void print_Illegal_exit_status(char *status_str)
{
	char *err = "exit: Illegal number: ";
	int err1_len = strlen(err);
	int err2_len = strlen(status_str);
	char *err_msg = allocate_memory(sizeof(char *) * (err1_len + err2_len + 2));

	strcpy(err_msg, err);
	strcat(err_msg, status_str);
	strcat(err_msg, "\n");
	/* Print error message */
	write(STDOUT_FILENO, err_msg, err1_len + err2_len + 1);
	free(err_msg);
}
