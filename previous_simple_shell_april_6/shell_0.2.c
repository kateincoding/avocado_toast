#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Command handlers */
char **parse_user_input(char *str_input);
int count_args(char *str_input, char *delimiter);
/*void execute_command(char **commands);*/
/* Utils */
void remove_new_line_char(char **args, int index);
/* Memory management */
void *allocate_memory(unsigned int bytes);
char *duplicate_string(char *str);
/* Error handlers */
void dispatch_error(char *msg, int status);

int main(int __attribute__((unused))ac, char **av)
{
	int read;
	char *buff = NULL;
	size_t buff_len = 0;

	int child_pid;
	char **commands;

	while(1)
	{
		/* Print console symbol */
		printf("#cisfun$ ");
		fflush(stdout);
		/* Read commands from console */
		read = getline(&buff, &buff_len, stdin);
		if (read == EOF)
		{
			free(buff);
			exit(0);
		}

		/* Fork parent process to execute the command */
		child_pid = fork();
		if (child_pid == -1)
			dispatch_error(av[0], 1);
		/* Fork parent process to execute the command */
		else if (child_pid == 0)
		{
			/* execute command */
			commands = parse_user_input(buff);
			execve(commands[0], commands, NULL);
			/* handle errors */
			dispatch_error(av[0], 1);
		}
		else
			wait (NULL);
	}

	/* Cleanup buffer memory */
	free(buff);

	return (0);
}

char **parse_user_input(char *str_input)
{
	int i, args_count = 0;
	char **args;
	char *token, *delimiter = " ";
	char *str_copy;

	if (str_input == NULL)
		dispatch_error("Error while parsing the command\n", 100);

	/* Count the number of arguments present in the input */
	args_count = count_args(str_input, delimiter);
	/* Allocate memory to hold eaach argument as a string */
	args = allocate_memory(sizeof(char *) * (args_count + 1));
	/* Store each argument as a string */
	str_copy = duplicate_string(str_input);
	for (i = 0; i < args_count; i++)
	{
		token = strtok(str_copy, delimiter);
		if (token == NULL)
			break;
		str_copy = NULL;
		/* store command as single string */
		args[i] = duplicate_string(token);
		/* Remove '\n' char from command */
		remove_new_line_char(args, i);
	}
	/* set the last element of array of arguments to NULL */
	args[i] = NULL;

	free(str_copy);

	return (args);
}

int count_args(char *str_input, char *delimiter)
{
	char *tkn;
	int count = 0;
	char *str_copy = duplicate_string(str_input);

	while((tkn = strtok(str_copy, delimiter)) != NULL)
	{
		count++;
		str_copy = NULL;
	}

	free(str_copy);

	return (count);
}

void *allocate_memory(unsigned int bytes)
{
	char *new_mem = malloc(bytes);

	if (new_mem == NULL)
		dispatch_error("Error while allocating memory\n", 100);

	return (new_mem);
}
/*
void execute_command(char **commands)
{
	 logica de pipes y redirection
}
*/

void dispatch_error(char *msg, int status)
{
	perror(msg);
	exit(status);
}

char *duplicate_string(char *str)
{
	char *str_copy = strdup(str);

	if (str_copy == NULL)
		dispatch_error("Error while making copy of string", 100);

	return (str_copy);
}

/**
 * remove_new_line_char - Replaces '\n' char with '\0'
 * @args: Array of commands
 * @index: Command index to be evaluated
 */
void remove_new_line_char(char **args, int index)
{
	int len = strlen(args[index]);

	if (len == 0)
		return;

	if (len == 1 && args[index][0] == '\n')
		args[index] = NULL;
	else if (args[index][len - 1] == '\n')
		args[index][len - 1] = '\0';
}