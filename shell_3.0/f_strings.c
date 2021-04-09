#include "shell.h"

/**
 * _strlen - returns the length of a string
 * @s: array
 * Return: length
 */
int _strlen(const char *s)
{
	int i = 0;

	while (s[i])
		i++;
	return (i);
}
/**
 * _strdup - copy a strings with a malloc
 * @s1: array
 * Return: pointer to the copy
 */
char *_strdup(const char *s1)
{
	char	*s2;
	size_t	i;

	i = 0;
	s2 = (char *)malloc(sizeof(char) * _strlen(s1) + 1);
	if (!s2)
		return (NULL);
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}