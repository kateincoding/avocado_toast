#ifndef __SHELL_H
#define __SHELL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* builtins */
void env(void);

/* own implementations */
char *_strtok(char *str, char *delimiter);

#endif /* __SHELL_H */