#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void error_socket(int error)
{
char* msg = NULL;
switch ( error )
    {
#ifndef USEMINIMAL
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
#endif
    default:
        msg = "socket()";
	break;
    }
fprintf( stderr, "error: %s\n", msg );
}

void error_malloc(int error)
{
char* msg = NULL;
switch ( error )
    {
#ifndef USEMINIMAL
    case ENOMEM:
        msg = "no memory";
	break;
#endif
    default:
        msg = "malloc()";
	break;
    }
fprintf( stderr, "error: %s\n", msg );
}

/*
void error_(int error)
{
char* msg = NULL;
switch ( error )
    {
#ifndef USEMINIMAL
#endif
    default:
        msg = "";
	break;
    }
fprintf( stderr, "error: %s\n", msg );
}
*/

/*void error_(int error)
{
char* msg = NULL;
switch ( error )
    {
#ifndef USEMINIMAL
#endif
    default:
        msg = "";
	break;
    }
fprintf( stderr, "error: %s\n", msg );
}
*/
