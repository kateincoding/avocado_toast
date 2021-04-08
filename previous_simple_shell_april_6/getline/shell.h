#ifndef SHELL_H
# define SHELL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int _getline(char **buffer, size_t *buf_size, FILE *restrict stream);

#endif
