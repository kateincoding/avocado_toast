#ifndef __SHELL_H
#define __SHELL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <stddef.h>

/* FLAGS */
#define F_BUFF 1
#define F_CMD_L 2
#define F_CMDS 4

/**
 * struct list_s - singly linked list
 * @str: string - (malloc'ed string)
 * @len: length of the string
 * @next: points to the next node
 *
 * Description: singly linked list node structure
 * for Holberton project
 */
typedef struct list_s
{
	char *str;
	unsigned int len;
	struct list_s *next;
} list_t;

/*linked lists*/
size_t print_list(const list_t *h);
list_t *add_node_end(list_t **head, const char *str);
void free_list(list_t *head);

/* Special functions */
void __attribute__((constructor)) build_dynamic_environ(void);
void __attribute__((destructor)) free_dynamic_environ(void);

/* builtins */
void env(void);
int _setenv(char *name, char *value);
int _unsetenv(char *name);
int _cd(char *path);

/* builtins utils */
int validate_env_name(char *name);
int is_valid_env_var_name(char *name);
int get_env_index(char *name);

/* own implementations */
char *_strtok(char *str, char *delimiter);
int _getline(char **buffer, size_t *buf_size, FILE *stream);

/* Command handlers */
void handle_PATH(char **commands);
char *getpath(char *dir, char *filename);
char **parse_user_input(char *str_input, char *delimiter);
int count_args(char *str_input, char *delimiter);

/* Memory management */
void *allocate_memory(unsigned int bytes);
char *duplicate_string(char *str);
void free_dbl_ptr(char **dbl_ptr);
void free_allocs(char *buff, char **cmds_list, char **commands, int flags);

/* handle_builtins */
int handle_builtins(char **commands);

/* handle_enter */
int handle_enter(char **commands);
/* handle comments in input */
char *handle_comment(char *str_input);

/* Exit handlers */
int handle_exit(char *buff, char **commands);
int get_exit_status(char *buff);
void print_Illegal_exit_status(char *status_str);

/* Error handlers */
void dispatch_error(char *msg, int status);

/* strings functions */
int _strlen(const char *s);
char *_strdup(const char *s1);

#endif /* __SHELL_H */
