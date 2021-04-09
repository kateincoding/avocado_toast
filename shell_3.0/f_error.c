#include "shell.h"

/**
 * dispatch_error - Dispatches an error
 * @msg: Message to print as error
 * @status: Integer representing the error code
*/
void dispatch_error(char *msg, int status)
{
	perror(msg);
	exit(status);
}