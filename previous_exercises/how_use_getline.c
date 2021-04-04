#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int read, cmp;
	char *line = NULL;
	size_t len = 0;

	while (1)
	{
		printf("$ ");
		if (read == getline(&line, &len, stdin) != -1)
		{
			puts("escribe una linea aqui /*entro*/");
			printf("el valor de line es = %s", line);
			printf("el valor de read get line  es = %d\n", read);
		}
		cmp = strcmp(line,"exit\n");
		if (read == EOF || strcmp(line,"exit\n") == 0)
		{
			puts("salimos");
			exit(0);
		}
			printf("resultado de strcmp (line, exit) = %i\n", cmp);
	}
	return (0);
}
