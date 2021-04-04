#include "shell.h"

#define B_SIZE 4

int _getline(char **buffer, size_t *buf_size, int fd)
{
	char *buf_tmp;
	size_t size;
	unsigned int i = 0, len = 0;

	if (*bufsize == 0)
		size = B_SIZE;

	if (*buffer == 0)
	{
		buf_tmp = malloc(sizeof(char) * size);
		if (!buf_tmp)
			return (write(2, "Segmentation Faul <error memory allocate>", 40));
		*buffer = buf_tmp;
	}
	/*escribir la ft memset*/
	buf_tmp = memset(buf_tmp, '\0', size);
	while (1)
	{
		r_fd = read(fd, buf_tmp + len, B_SIZE);
		if (r_fd >= 0)
			i = len, len += r_fd;
		else if ( r_fd == -1 || r_fd == 0)
			return (-1);
		if (len >= size)
		{
			sizeold = size;
			size += B_SIZE;
		}
	

}
