#include "shell.h"

/**
 * env - Prints the environment variables to stdout
*/
void env(void)
{
	int i;
	char **env = __environ;

	for (i = 0; env[i]; i++)
	{
		write(STDOUT_FILENO, env[i], strlen(env[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
}

/**
 * _setenv - Sets or adds an environment variable
 * @name: Name for the new env variable
 * @value: Value for the new env variable
 *
 * Return: 1 on success, -1 on error
 */
int _setenv(char *name, char *value)
{
	int env_index, new_var_len;

	if (validate_env_name(name) == -1)
		return (-1);

	env_index = get_env_index(name);
	if (env_index == -1)
	{/* var doen't exist, SO CREATE IT */
		int env_count = 0;

		while (__environ[env_count] != NULL)
			env_count++;

		__environ = realloc(__environ, sizeof(char *) * (env_count + 2));
		if (__environ == NULL)
			dispatch_error("Error while reallocating memory for new env var", 2);

		/* The new value will be stored at index env_count */
		env_index = env_count;
		/* last value For the new env var needs to be NULL */
		__environ[env_count + 1] = NULL;
	}
	else
	{
		/* var exists, so overwrite it's value */
		free(__environ[env_index]);
	}

	new_var_len = strlen(name) + strlen(value) + 2;
	/* store the env var either if it exists or it needs to be overwritten */
	__environ[env_index] = allocate_memory(sizeof(char) * new_var_len);
	strcpy(__environ[env_index], name);
	strcat(__environ[env_index], "=");
	strcat(__environ[env_index], value);

	return (1);
}

/**
 * _unsetenv - Removes an evironment variable
 * @name: Name for the new env variable
 *
 * Return: 1 on success, -1 on error
 */
int _unsetenv(char *name)
{
	int env_index, i;

	env_index = get_env_index(name);
	if (env_index >= 0)
	{/* var exists, We can unset it */
		free(__environ[env_index]);

		for (i = env_index; __environ[i] != NULL; i++)
			__environ[i] = __environ[i + 1];

		return (1);
	}

	/* Var doesn't exist, we can print error or do nothing */

	return (1);
}

/**
 * _cd - Changes the current directory of the process
 * @path: Path to wich change the working directory
 *
 * Return: 1 on success, -1 on error
*/
int _cd(char *path)
{
	char buff[1024];
	char *oldpwd, *err_msg;

	if (strcmp(path, "-") == 0)
		path = getenv("OLDPWD");

	if (path == NULL)
	{
		err_msg = "Error: cd: OLDPWD not set\n";
		write(STDOUT_FILENO, err_msg, strlen(err_msg));
		return (-1);
	}

	/* Needed to avoid reading on freed memory */
	path = duplicate_string(path);
	/* store this dir in case of update */
	oldpwd = getcwd(buff, 1024);
	if (oldpwd == NULL)
	{
		err_msg = "Error while getting current directory\n";
		free(path);
		write(STDOUT_FILENO, err_msg, strlen(err_msg));
		return (-1);
	}
	/* Try to change the current dir */
	if (chdir(path) == -1)
	{
		free(path);
		perror("Error while changing dir");
		return (-1);
	}

	/* Update env variables */
	_setenv("OLDPWD", oldpwd);
	_setenv("PWD", path);

	free(path);

	return (1);
}
