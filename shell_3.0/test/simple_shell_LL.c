#include "shell.h"

/*function with all the logical part that will work with the main */
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
		/* Handling_semicolon and executes inside of the function */
		execute_handling_semicolon(buff, read, av[0]);
	}
	/* Free buffer memory */
	free(buff);
	return (0);
}

/**
 * execute_handling_semicolon - Handle semicolon and executes inside of it
 * @buff: first buffer that functions read
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * Return: 0 on success
*/
void execute_handling_semicolon(char *buff, int read, char *first_av)
{
	char *block = _strtok(buff, ";");
	list_t *head, *tmp_head;

	head = NULL;
	if (block[0] == '\n')
	{
/*		puts(" retorna salto de linea");*/
		return;
	}
	/* Create a linked list for the blocks and allocate in the first while */
	while (block != NULL)
        {       
                /*printf("block = '%s'\n", block);*/
				add_node_end(&head, block);
                block = _strtok(NULL, ";");
                /*printf("block after strtok = '%s'\n", block);*/
        }
	/*print_list(head);*/
	/* asignamos un tmp head para no perder el head */
	tmp_head = &*head;

	while (tmp_head)
	{
		/*char *tmp = _strdup(block);*/
		execute_commands(tmp_head->str, read, first_av);
		/*printf("block = '%s'\n", block);*/
		block = _strtok(NULL, ";");
		/*printf("block after strtok = '%s'\n", block);*/
		/*free (tmp);*/
		tmp_head = tmp_head->next;
	}
	free_list(tmp_head);
	free_list(head);
	head = NULL;	
}

/**
 * execute_commands - Fork and create commands, child process and executed
 * @buff: first buffer that function read
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * Return: 0 on success
*/
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
