#include "shell.h"

/* Command handlers */
char **parse_user_input(char *str_input);
int count_args(char *str_input, char *delimiter);
/*void execute_command(char **commands);*/
/* Utils */
void remove_new_line_char(char **args, int index);
/* Memory management */
void *allocate_memory(unsigned int bytes);
char *duplicate_string(char *str);
void free_dbl_ptr(char **dbl_ptr);
/* Error handlers */
void dispatch_error(char *msg, int status);

void handle_PATH(char **commands);
char *getpath(char *dir, char *filename);

int handle_builtins(char **commands);

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

	int child_pid;
	char **commands = NULL;

	while (1)
	{
		/* Print console symbol */
		printf("#cisfun$ ");
		fflush(stdout);
		/* Read commands from console */
		read = getline(&buff, &buff_len, stdin);
		if (read == EOF || strcmp(buff, "exit\n") == 0)
		{
			free(buff);
			free_dbl_ptr(commands);
			exit(0);
		}

		if (buff[0] == '\n' && buff[1] == '\0')
			continue;

		/* Fork parent process to execute the command */
		child_pid = fork();
		if (child_pid == -1)
			dispatch_error(av[0], 1);
		/* Fork parent process to execute the command */
		else if (child_pid == 0)
		{
			int is_builtin;
			/* execute command */
			commands = parse_user_input(buff);
			/* Check if the command is a builtin */
			is_builtin = handle_builtins(commands);
			if (is_builtin == 1)
			{
				free(buff);
				free_dbl_ptr(commands);
				exit(0);
			}
			/* Handle non builtin commands */
			handle_PATH(commands);
			execve(commands[0], commands, NULL);
			/* handle errors */
			free(buff);
			free_dbl_ptr(commands);
			dispatch_error(av[0], 1);
		}
		else
			wait(NULL);
	}

	/* Cleanup buffer memory */
	free(buff);

	return (0);
}

/**
 * parse_user_input - Buidls an array of strings as arguments
 * @str_input: Command input given by the user
 *
 * Return: Array of strings
*/
char **parse_user_input(char *str_input)
{
	int i, args_count = 0;
	char **args;
	char *token, *tkn_ptr, *delimiter = " ";
	char *str_copy;

	if (str_input == NULL)
		dispatch_error("Error while parsing the command\n", 100);

	/* Count the number of arguments present in the input */
	args_count = count_args(str_input, delimiter);
	/* Allocate memory to hold eaach argument as a string */
	args = allocate_memory(sizeof(char *) * (args_count + 1));
	/* Store each argument as a string */
	str_copy = duplicate_string(str_input);
	tkn_ptr = str_copy;
	for (i = 0; i < args_count; i++)
	{
		token = _strtok(tkn_ptr, delimiter);
		if (token == NULL)
			break;
		tkn_ptr = NULL;
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

/**
 * count_args - Counts the number of arguments in a command string
 * @str_input: Command as a string
 * @delimiter: Delimiter by wich to separate the args
 *
 * Return: Number of arguments present in str_input
*/
int count_args(char *str_input, char *delimiter)
{
	char *tkn, *tkn_ptr;
	int count = 0;
	char *str_copy = duplicate_string(str_input);

	tkn_ptr = str_copy;
	while ((tkn = _strtok(tkn_ptr, delimiter)) != NULL)
	{
		count++;
		tkn_ptr = NULL;
	}

	free(str_copy);

	return (count);
}

/**
 * allocate_memory - Manages the allocation of memory
 * @bytes: Number representing the size in bytes to allocate
 *
 * Return: Pointer to the newly allocated memory
*/
void *allocate_memory(unsigned int bytes)
{
	char *new_mem = malloc(bytes);

	if (new_mem == NULL)
		dispatch_error("Error while allocating memory\n", 100);

	return (new_mem);
}

/*
 * void execute_command(char **commands)
 * {
 *    logica de pipes y redirection
 * }
*/

/**
 * dispatch_error - Dispatches an error
 * @msg: Message to print as error
 * @status: Integer representing the error code
*/
void dispatch_error(char *msg, int status)
{
	perror(msg);
	exit(status);
}

/**
 * duplicate_string - Duplicates a given string
 * @str: String to dulicate
 *
 * Return: Pointer to the address of the newly duplicated string
*/
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

/**
 * handle_PATH - Checks if the command to execute could be found in PATH's dirs
 * @commands: Array of strings containing the command and args
*/
void handle_PATH(char **commands)
{
	char *path_dirs, *path;
	char *tkn, *tkn_ptr;
	char *str_copy;

	if (
		commands == NULL || commands[0] == NULL ||
		commands[0][0] == '\0' || commands[0][0] == '/'
	)
		return;

	path_dirs = getenv("PATH");
	if (path_dirs == NULL)
		dispatch_error("Error", 100);

	str_copy = duplicate_string(path_dirs);
	tkn_ptr = str_copy;
	while (1)
	{
		tkn = _strtok(tkn_ptr, ":");
		if (tkn == NULL)
			break;
		tkn_ptr = NULL;

		path = getpath(tkn, commands[0]);
		if (access(path, F_OK) != -1)
		{
			free(commands[0]);
			commands[0] = path;
			break;
		}
		free(path);
	}

	free(str_copy);
}

/**
 * getpath - Creates a string representing a full path to file
 * @dir: String representing a directory path
 * @filename: Name of the file we are looking for
 *
 * Return: String as a full path to "filename"
*/
char *getpath(char *dir, char *filename)
{
	int dir_len = strlen(dir);
	int filename_len = strlen(filename);
	char *path;

	path = allocate_memory(sizeof(char *) * (dir_len + filename_len + 2));

	strcpy(path, dir);
	strcat(path, "/");
	strncat(path, filename, filename_len + 1);

	return (path);
}

/**
 * free_dbl_ptr - Frees the memory pointed to by a double pointer
 * @dbl_ptr: Double pointer
*/
void free_dbl_ptr(char **dbl_ptr)
{
	int i;

	if (dbl_ptr == NULL)
		return;

	for (i = 0; dbl_ptr[i]; i++)
		free(dbl_ptr[i]);

	free(dbl_ptr);
}

/**
 * handle_builtins - Perfoms the builtin in case the command is one
 * @commands: Command as a string given by the user
 *
 * Return: 1 if the commad is a builtin, 0 otherwise
*/
int handle_builtins(char **commands)
{
	if (strcmp(commands[0], "env") == 0)
	{
		env();
		return (1);
	}

	return (0);
}
