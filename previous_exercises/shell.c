#include <stdio.h>


int main(){
	/* definicion inicial, cambiar por fx */
	char commands[100];

	while(1)
	{
		printf("$ ");
		len_read = getline(&buff, &buff_len, stdin);
		char commands[len_read + 1];
		
		if (len_read == EOF)
			break;
		IF (*buff == 'exit')
			break;
	}

	int pid = fork();

	if (pid = -1)
		/*error*/exit(98);
	else if (pid == 0)
	{
		execute_child(commands)
	}
	free(buff);

	return (0);
}i
