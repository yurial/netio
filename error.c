#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void error_socket(int error)
{
char* msg = NULL;
switch ( error )
    {
    case EACCES:
        msg = "Permission  to  create  a  socket  of the specified type and/or protocol is denied.";
	break;
    case EPROTONOSUPPORT:
	msg = "The implementation does not support the specified address family.";
        break;
    case ENFILE:
        msg = "Invalid flags in type.";
	break;
    case EMFILE:
        msg = "Process file table overflow.";
        break;
    case ENOBUFS:
    case ENOMEM:
        msg = "Insufficient memory is available. The socket cannot be created until sufficient resources are freed.";
        break;
    case EINVAL:
        msg = "Unknown protocol, or protocol family not available.";
        break;
    }
fprintf( stderr, "error: %s\n", msg );
exit( EXIT_FAILURE );
}
