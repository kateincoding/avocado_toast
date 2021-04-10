#include "shell.h"

/**
 * dispatch_error - Dispatches an error
 * @msg: Message to print as error
 * @status: Integer representing the error code
*/
void dispatch_error(char *msg, int status)
{
	/** errno value       Error **/
             /*1 Operation not permitted */
             /*2 No such file or directory */
             /*3 No such process */
             /*4 Interrupted system call */
             /*5 I/O error */
             /*6 No such device or address */
             /*7 Argument list too long */
             /*8 Exec format error */
             /*9 Bad file number */
            /*10 No child processes */
            /*11 Try again */
            /*12 Out of memory */
            /*13 Permission denied */

	perror(msg);
	exit(status);
}