#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*ft to return number of ' ' in a function */
int count_spaces(char *str)
{
	int i, count = 0;
	for(i = 0; str[i]; i++)
	{
		if (str[i] == ' ')
			count++;
	}
	return (count);
}

/* ft to execute the child process with a line like as input */
void execute_child(char *line)
{
	int len_line = strlen(line);
	/*falta agregar a la fx que elimine los espacios extra y si hay al final */
	int _argc = count_spaces(line) + 1;
	char *_argv[_argc];
	printf ("execute function execute_child well \n");
}

int main()
{
	int read, cmp, pid;
	char *line = NULL;
	size_t len = 0;

	while(1)
	{
		line = NULL;
		len = 0;

		printf("$ ");
		read = getline(&line, &len, stdin);
		/* it can return -1 if it is failure */
		if (read == -1)
			exit(100);
		else 
		{
			puts("escribe una linea aqui /*entro*/");
			printf("el valor de line es = %s", line);
			printf("el valor de read get line  es = %d\n", read);
		}
		if (read == EOF || strcmp(line,"exit\n") == 0)
		{
			free(line);
			puts("salimos con eof o exit101");
			exit(100);
		}

		pid = fork();
		if (pid == 0)
			execute_child(line);
		else
			wait (NULL);
		free(line);
	}
	return (0);
}