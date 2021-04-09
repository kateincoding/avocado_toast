#include "shell.h"

/**
 * handle_enter - Check if no command was entered
 * @buff: User's input
 *
 * Return: 1 if no command was entered(just spaces and ENTER), 0 otherwise
*/
int handle_enter(char **commands)
{
	if (commands[0] == NULL || strcmp(commands[0], "\n") == 0)
		return (1);

	return (0);
}