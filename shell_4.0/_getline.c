#include "shell.h"
#include <stdio.h>

/* I selected start with 4 because is the smallest command: ls/n/0 */
#define BUFF_SIZE 256

char *_memset(char *str, char a, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		str[i] = a;
	return (str);
}

int _getline(char **buffer, size_t *buf_size, FILE *stream)
{
	char *buf_tmp;
	size_t size;
	unsigned int i = 0, len = 0;
	int r, fd;

	stream = stdin;
	/* case: we don't have nothing to read */
	if(buffer == NULL || buf_size == NULL)
	{
		printf("Error\n");
		exit(100);
	}
	/* initialize with a constant var BUFF_SIZE */
	if (*buf_size == 0)
		size = BUFF_SIZE;
	while (1)
	{
		/* caso inicial buffer = 0 */
		buf_tmp = malloc(sizeof(char) * size);
		if (!buf_tmp)
			return (write(2, "Segmentation Faul <error memory allocate>", 40));
		*buffer = buf_tmp;
		/* set buf_tmp with '\0' , no es necesario*/
		buf_tmp = _memset(buf_tmp, '\0', size);
		if (stream == stdin)
			fd = 0;
		/* read(int fd, void *buf, size_t count) each BUFF_SIZE */
		r = read(fd, buf_tmp + len, BUFF_SIZE);
		/* next: we read read + len */
		if (r >= 0)
			i = len, len += r;
		/* case of error */
		else if ( r == -1 || r == 0)
			return (-1);
		/* case len > size when len != 0*/
		if (len >= size)
		{
			/* we will read BUFF_SIZE ++ */
			size += BUFF_SIZE;
			/*  realloc() function changes the size of the memory block pointed to by ptr to size bytes.*/
			buf_tmp = realloc(buf_tmp, size);
			/* If realloc() fails the original block is left untouched; it is not freed or moved.*/
			if (!buf_tmp)
				return (write(1, "error of memory allocation", 100));
		}
		while(i < len)
		{
			if (buf_tmp[i] == '\n')
			{
				/* we found the complete buffer, so we realloc to the new size*/
				*buf_size = i + 1;
				buf_tmp = realloc(buf_tmp, *buf_size);
				if (!buf_tmp)
					return (write(1, "error of memory allocation", 100));
				*buffer = buf_tmp;
				/*if (buf_tmp == "^D\n")
					return(EOF); */
				/* printf("\ni = %d || buffer = '%s' || buf_size = %zu\n", i, *buffer, *buf_size); */
				return (*buf_size);
			}
			i++;
		}
	}
	return (len);
}
