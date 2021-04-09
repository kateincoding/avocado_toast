#include "shell.h"

/*function with all the logical part that will work with the main */
void execute_commands(char *buff, char **cmds_list, char *cmd,
											int read, char *first_av);
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

void execute_handling_and_or(char *buff, int read, char *first_av)
{
	int i;
	char **cmds_list = parse_user_input(buff, ";");

	for (i = 0; cmds_list[i] != NULL; i++)
		execute_commands(buff, cmds_list, cmds_list[i], read, first_av);

	free_dbl_ptr(cmds_list);
}
*/

/**
 * execute_handling_semicolon - Handle semicolon and executes inside of it
 * @buff: first buffer that functions read
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * Return: 0 on success
*/
void execute_handling_semicolon(char *buff, int read, char *first_av)
{
        int i, i_prev = 0, j = 0; flag_and = 1, success = -1;
        char **cmds_list = parse_user_input(buff, ";");

        for (i = 0; cmds_list[i] != NULL; i++)
	{
		if ( cmds_list[i] == '||' || cmds_list[j] == '&&')
		{
			if (cmds_list[i] == '||')
				flag_and == 0;

			while (cmds_list[i] != '||' || cmds_list[i] != '&&' || cmds_list[i] != '\0')
				i++;

			char *buff_input[i - iprev + 1];

			buff_input[iprev - i + 1] == '\0';
			/*excute and check if it is success */
                	success = execute_commands(buff_input, cmds_list, cmds_list[i], read, first_av);
			if (success == 0)
			{
				/*search the && */
				j = i;
				while(cmds_list[i] != '\0')
				{
					/* mark the first char */
					if (cmds_list[i] != '||')
						j = i;
					if (cmds_list[i] != '&&')
					{
						j = i + 2;
						/* escribirlo en una fx, se repite y que i e i ++ sea |*/
						while(cmds_list[i] != '||' || cmds_list[i] != '&&' || cmds_list[i] != '\0')
							i++;
						char *buff_input2[i - iprev + 1];

                        			buff_input2[iprev - i + 1] == '\0';
						execute_commands(buff_input2, cmds_list, cmds_list[i], read, first_av);
					}	
					i++;
					j = i;
				}
			}
			/*else , repeat the loop because we need to run the first block */
			free(buff_input); cmds_list[i] != '||' || cmds_list[i] != '&&' || cmds_list[i] != '\0'; j++)
		}
		i_prev = i;
		i++;
	}
        free_dbl_ptr(cmds_list);
}

/**
 * execute_commands - Fork and create commands, child process and executed
 * @buff: first buffer that function read
 * @cmds_list: List of commands
 * @cmd: Single command as a string
 * @read: return of read (open with getline)
 * @first_av: av[0]
 * Return: flag 0 on success, -1 error;
*/
int execute_commands(char *buff, char **cmds_list, char *cmd,
	int read, char *first_av)
{
	int child_pid, flag = -1;
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
		return;
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
		flag = handle_PATH(commands);
		/* execute command */
		execve(commands[0], commands, NULL);
		/* free memory */
		free_allocs(buff, cmds_list, commands, F_BUFF | F_CMD_L | F_CMDS);
		/* handle errors */
		dispatch_error(first_av, 1);
	}

	wait(NULL);
	free_dbl_ptr(commands);
	return(flag);
}
