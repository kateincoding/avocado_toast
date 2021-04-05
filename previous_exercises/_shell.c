#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*_strchr(const char *s, int c)
{
	size_t i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return ((char*)&s[i]);
		i++;
	}
	if (c == '\0')
		return ((char*)(&s[i]));
	return (NULL);
}

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


void create_argvs(int argc, char **array, char *line)
{
	char *next_argv;
	unsigned int i = 0, i_prev = 0, j = 0;
	int len_mem = 0, len = strlen(line);

	for(j = 0; j <= argc; j++)
	{
		if (line [i] == ' ')
		{
			len_mem = i - i_prev;
			array[j] = _memalloc(i - i_prev);
			for (int k = 0; k < (i - i_prev); k++)
			{
				array[j][k] = line[i_prev];
				i_prev++;
			}
			array[j][i - i_prev] = '\0';
			/* array[j] = _memcpy(array[j], line, i - i_prev + 1); */
			/*falta*/
		}
		/* agregar caso termine en espacios */
		i_prev = i;
		i++;
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
	create_argvs(_argc, _argv, line);
	execve(_argv[0], _argv, NULL);
	/* marcador */
	printf (" argv = %s \n", _argv[0]);
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