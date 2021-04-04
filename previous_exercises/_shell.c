#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void	*_memalloc(size_t size)
{
	unsigned char *usize;

	usize = malloc(size);
	if (usize == NULL)
		return (NULL);
	while (size)
		usize[--size] = 0;
	return ((void *)usize);
}

void	*_memcpy(void *dst, const void *src, size_t n)
{
	size_t	idx;

	idx = 0;
	if (!src && !dst)
		return (NULL);
	while (idx < n)
	{
		((unsigned char*)dst)[idx] = ((unsigned char*)src)[idx];
		++idx;
	}
	return (dst);
}


void create_argvs(char **array, char *line)
{
	char *next_argv = strchr(line, ' ');
	unsigned int i, j = 0;

	for(i = 0; line[i], i++)
	{
		if (line [i] == ' ')
		{
			array[j][i] = _memalloc(i);
			*array[j] = _memcpy(&array[j], line, i - 1);
			/*falta*/
		}
	}
}

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
/* paso 2 cambiar la fx, cuando sea un input por la tuberia */
void execute_child(char *line)
{
	int len_line = strlen(line);
	/*falta agregar a la fx que elimine los espacios extra y si hay al final */
	int _argc = count_spaces(line) + 1;
	char *_argv[_argc];
	/* fx que aloca los argv en el array */
	create_argvs(*_argv, line);
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
		else if (read == EOF || strcmp(line,"exit\n") == 0)
		{
			free(line);
			puts("salimos con eof o exit101");
			exit (0);
		}
		else 
		{
			puts("escribe una linea aqui /*entro*/");
			printf("el valor de line es = %s\n", line);
			printf("el valor de read get line  es = %d\n", read);
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