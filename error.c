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

void error_accept(int error)
{
char* msg = NULL;
switch ( error )
    {
#ifndef USEMINIMAL
    case EAGAIN:
        msg = "The socket is marked non-blocking and no connections are present to be accepted. POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.";
	break;
    case EBADF:
        msg = "The descriptor is invalid.";
	break;
    case ECONNABORTED:
        msg = "A connection has been aborted.";
	break;
    case EFAULT:
        msg = "The addr argument is not in a writable part of the user address space.";
	break;
    case EINTR:
        msg = "The system call was interrupted by a signal that was caught before a valid connection arrived; see signal(7).";
	break;
    case EINVAL:
        msg = "Socket is not listening for connections, or addrlen is invalid (e.g., is negative).\n(accept4()) invalid value in flags.";
	break;
    case EMFILE:
        msg = "The per-process limit of open file descriptors has been reached.";
	break;
    case ENFILE:
        msg = "The system limit on the total number of open files has been reached.";
	break;
    case ENOBUFS:
        msg = "No busfs.";
	break;
    case ENOMEM:
        msg = "Not enough free memory.  This often means that the memory allocation is limited by the socket buffer limits, not by the system memory.";
	break;
    case ENOTSOCK:
        msg = "The descriptor references a file, not a socket.";
	break;
    case EOPNOTSUPP:
        msg = "The referenced socket is not of type SOCK_STREAM.";
	break;
    case EPROTO:
        msg = "Protocol error.";
	break;
    case EPERM:
        msg = "Firewall rules forbid connection.";
	break;
    case ENOSR:
    case ESOCKTNOSUPPORT:
    case EPROTONOSUPPORT:
    case ETIMEDOUT:
#endif
    default:
        msg = "accept()";
	break;
    }
fprintf( stderr, "error: %s\n", msg );
}

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
