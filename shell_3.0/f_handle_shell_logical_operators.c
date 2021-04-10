#include "shell.h"

/**
 * handling_semicolon_and_operators - Handle semicolon and logical op
 * @buff: first buffer that functions read
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * Return: 0 on success
*/
void handling_semicolon_and_operators(char *buff, int read, char *first_av)
{
	int i;
	char **cmds_list = parse_user_input(buff, ";");

	for (i = 0; cmds_list[i] != NULL; i++)
		handling_or(cmds_list[i], read, first_av);
	free_dbl_ptr(cmds_list);
}

/**
 * handling_or - Handle || logical part
 * @buff_semicolon: first buffer that functions read
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * Return: 0 on success
*/
void handling_or(char *buff_semicolon, int read, char *first_av)
{
	int i, result = 0, flag, prev_flag = -1;
	char **cmds_list_2 = parse_user_input(buff_semicolon, "||");

	for (i = 0; cmds_list_2[i] != NULL; i++)
	{
		flag = handling_and(cmds_list_2[i], read, first_av, prev_flag);
		/* record de last*/
		prev_flag = flag;
	}
	free_dbl_ptr(cmds_list_2);
}

/**
 * handling_and - Handle && logical part and executes inside of it
 * @buff_or: first buffer that functions read
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * @prev_flag: last flag of ||, it is important to print or not
 * Return: 0 on success
*/
int handling_and(char *buff_or, int read, char *first_av, int prev_flag)
{
	int j = 0, result, flag = 1;
	char **cmds_list_3 = parse_user_input(buff_or, "&&");

	/* logical part: if the last || is success, */
	/*	next -> &&; if not exist &&, return */
	if (prev_flag == 0)
	{
		j++;
		if (cmds_list_3[j] == NULL)
		{
			flag == prev_flag;
			return (-1);
		}
	}

	for (; cmds_list_3[j] != NULL; j++)
	{
		flag = execute_commands(buff_or, cmds_list_3,
									cmds_list_3[j], read, first_av);
		prev_flag = flag;
	}
		/* record de last result , estudiar el caso 0 */
	free_dbl_ptr(cmds_list_3);
	return (flag);
}

/**
 * execute_commands - Fork and create commands, child process and executed
 * @buff: first buffer that function read
 * @cmds_list: List of commands
 * @cmd: Single command as a string
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * Return: 0 on success
*/
int execute_commands(char *buff, char **cmds_list, char *cmd,
	int read, char *first_av)
{
	int child_pid;
	int flag = 0;
	char **commands;

	/* Generate array of commands */
	commands = parse_user_input(cmd, " ");
	if (read == EOF)
	{
		free_allocs(buff, cmds_list, commands, F_BUFF | F_CMD_L | F_CMDS);
		exit(0);
	}
	/* Exit error, ENTER, and builtins */
	if (handle_exit(cmd, commands) == -1 ||
			handle_enter(commands) == 1	||
			handle_builtins(commands) == 1)
	{
		free_dbl_ptr(commands);
		return (-1);
	}
	/* have a flag when the execution is an error (||)*/
	if (handle_PATH(commands) == -1)
		flag = -1;
	/* check if we can only run for positives */
	/* Fork parent process to execute the command */
	child_pid = fork();
	if (child_pid == -1)
		dispatch_error(first_av, 1);
	else if (child_pid == 0)
	{
		/* Search command using the PATH env variable */
		handle_PATH(commands);
		/* execute command */
		execve(commands[0], commands, NULL);
		/* free memory */
		free_allocs(buff, cmds_list, commands, F_BUFF | F_CMD_L | F_CMDS);
		/* handle errors */
		dispatch_error(first_av, 1);
	}
	wait(NULL);
	free_dbl_ptr(commands);
	return (flag);
}
