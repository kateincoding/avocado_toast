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

/**
 * _strlen - returns the length of a string
 * @s: array
 * Return: length
 */
int _strlen(const char *s)
{
	int i = 0;

	while (s[i])
		i++;
	return (i);
}

/**
 * add_node_end - add a new node at the end of the linked list
 * @head: pointer to the first node
 * @str: content of the string for the first node
 * Return: pointer to the first node
 */
list_t *add_node_end(list_t **head, const char *str)
{
	list_t *end_node, *tmp;

	end_node = malloc(sizeof(list_t));

	if (end_node == NULL)
		return (NULL);

	end_node->str = strdup(str);
	end_node->len = _strlen(str);
	end_node->next = NULL;

	if (*head == NULL)
	{
		*head = end_node;
		return (*head);
	}
	tmp = *head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = end_node;
	return (end_node);

}

/**
 * print_list - Print the lists
 * @h: list with format list_t
 * Return: number of nodes of the list
 */
size_t print_list(const list_t *h)
{
	unsigned int number_nodes = 0;

	while (h != NULL)
	{
		if (h->str)
			printf("[%i] %s\n", h->len, h->str);
		else
			printf("[0] (nil)\n");
		h = h->next;
		number_nodes++;
	}
	return (number_nodes);
}

char	*_strdup(const char *s1)
{
	char	*s2;
	size_t	i;

	i = 0;
	s2 = (char *)malloc(sizeof(char) * _strlen(s1) + 1);
	if (!s2)
		return (NULL);
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

/**
 * free_list - functions that free a linkedlist
 * @head: head of first node
 * Return: void -> free the linked list
 */
void free_list(list_t *head)
{
	if (head == 0)
		return;
	free_list(head->next);
	free(head->str);
	free(head);
}

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

