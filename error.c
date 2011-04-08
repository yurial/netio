#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void error_socket(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
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
    #endif
    fprintf( stderr, "socket() error #%d: %s\n", error, msg );
#endif
}

void error_malloc(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case ENOMEM:
            msg = "no memory";
            break;
        }
    #endif
    fprintf( stderr, "malloc() error #%d: %s\n", error, msg );
#endif
}

void error_bind(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EACCES:
            msg = "The address is protected, and the user is not the superuser.\nSearch permission is denied on a component of the path prefix. (See also path_resolution(7).)";
            break;
        case EADDRINUSE:
            msg = "The given address is already in use.";
            break;
        case EBADF:
            msg = "sockfd is not a valid descriptor.";
            break;
        case EINVAL:
            msg = "The socket is already bound to an address.\nThe addrlen is wrong, or the socket was not in the AF_UNIX family.";
            break;
        case ENOTSOCK:
            msg = "sockfd is a descriptor for a file, not a socket.";
            break;
        case EADDRNOTAVAIL:
            msg = "A nonexistent interface was requested or the requested address was not local.";
            break;
        case EFAULT:
            msg = "Addr points outside the user's accessible address space.";
            break;
        case ELOOP:
            msg = "Too many symbolic links were encountered in resolving addr.";
            break;
        case ENAMETOOLONG:
            msg = "Addr is too long.";
            break;
        case ENOENT:
            msg = "The file does not exist.";
            break;
        case ENOMEM:
            msg = "Insufficient kernel memory was available.";
            break;
        case ENOTDIR:
            msg = "A component of the path prefix is not a directory.";
            break;
        case EROFS:
            msg = "The socket inode would reside on a read-only file system.";
            break;
        }
    #endif
    fprintf( stderr, "bind() error #%d: %s\n", error, msg );
#endif
}

void error_accept(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
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
#ifdef ENOSR
	case ENOSR:
#endif
        case ESOCKTNOSUPPORT:
        case EPROTONOSUPPORT:
        case ETIMEDOUT:
            msg = "In addition, network errors for the new socket and as defined for the protocol may be returned. Various Linux kernels can return other errors such as ENOSR, ESOCKTNOSUPPORT, EPROTONOSUPPORT, ETIMEDOUT. The value ERESTARTSYS may be seen during a trace.";
            break;
        }
    #endif
    fprintf( stderr, "accept() error #%d: %s\n", error, msg );
#endif
}

void error_listen(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EADDRINUSE:
            msg = "Another socket is already listening on the same port.";
            break;
        case EBADF:
            msg = "The argument sockfd is not a valid descriptor.";
            break;
        case ENOTSOCK:
            msg = "The argument sockfd is not a socket.";
            break;
        case EOPNOTSUPP:
            msg = "The socket is not of a type that supports the listen() operation.";
        break;
        }
    #endif
    fprintf( stderr, "listen() error #%d: %s\n", error, msg );
#endif
}

void error_connect(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EACCES:
            msg = "For Unix domain sockets, which are identified by pathname: Write permission is denied on the socket file, or search permission is denied for one of the directories in the path prefix. (See also path_resolution(7).)\nThe user tried to connect to a broadcast address without having the socket broadcast flag enabled or the connection request failed because of a local firewall rule.";
            break;
        case EADDRINUSE:
            msg = "Local address is already in use.";
            break;
        case EAFNOSUPPORT:
            msg = "The passed address didn't have the correct address family in its sa_family field.";
            break;
        case EAGAIN:
            msg = "No more free local ports or insufficient entries in the routing cache. For AF_INET see the description of /proc/sys/net/ipv4/ip_local_port_range ip(7) for information on how to increase the number of local ports.";
            break;
        case EALREADY:
            msg = "The socket is non-blocking and a previous connection attempt has not yet been completed.";
            break;
        case EBADF:
            msg = "The file descriptor is not a valid index in the descriptor table.";
            break;
        case ECONNREFUSED:
            msg = "No-one listening on the remote address.";
            break;
        case EFAULT:
            msg = "The socket structure address is outside the user's address space.";
            break;
        case EINPROGRESS:
            msg = "The socket is non-blocking and the connection cannot be completed immediately. It is possible to select(2) or poll(2) for completion by selecting the socket for writing. After select(2) indicates writability, use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET to determine whether connect() completed successfully (SO_ERROR is zero) or unsuccessfully (SO_ERROR is one of the usual error codes listed here, explaining the reason for the failure).";
            break;
        case EINTR:
            msg = "The system call was interrupted by a signal that was caught; see signal(7).";
            break;
        case EISCONN:
            msg = "The socket is already connected.";
            break;
        case ENETUNREACH:
            msg = "Network is unreachable.";
            break;
        case ENOTSOCK:
            msg = "The file descriptor is not associated with a socket.";
            break;
        case ETIMEDOUT:
            msg = "Timeout while attempting connection. The server may be too busy to accept new connections. Note that for IP sockets the timeout may be very long when syncookies are enabled on the server";
            break;
        }
    #endif
    fprintf( stderr, "connect() error #%d: %s\n", error, msg );
#endif
}

void error_dup2(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EBADF:
            msg = "oldfd isn't an open file descriptor, or newfd is out of the allowed range for file descriptors.";
            break;
        case EBUSY:
            msg = "(Linux only) This may be returned by dup2() or dup3() during a race condition with open(2) and dup().";
            break;
        case EINTR:
            msg = "The dup2() or dup3() call was interrupted by a signal; see signal(7).";
            break;
        case EINVAL:
            msg = "(dup3()) flags contain an invalid value.  Or, oldfd was equal to newfd.";
            break;
        case EMFILE:
            msg = "The process already has the maximum number of file descriptors open and tried to open a new one.";
            break;
        }
    #endif
    fprintf( stderr, "dup2() error #%d: %s\n", error, msg );
#endif
}

void error_close(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EBADF:
            msg = "fd isn't a valid open file descriptor.";
            break;
        case EINTR:
            msg = "The close() call was interrupted by a signal; see signal(7).";
            break;
        case EIO:
            msg = "An I/O error occurred.";
            break;
        }
    #endif
    fprintf( stderr, "close() error #%d: %s\n", error, msg );
#endif
}

void error_read(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EAGAIN:
            msg = "The file descriptor fd refers to a file other than a socket and has been marked non-blocking (O_NONBLOCK), and the read would block.\nThe file descriptor fd refers to a socket and has been marked non-blocking (O_NONBLOCK), and the read would block.  POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.";
            break;
        case EBADF:
            msg = "fd is not a valid file descriptor or is not open for reading.";
            break;
        case EFAULT:
            msg = "buf is outside your accessible address space.";
            break;
        case EINTR:
            msg = "The call was interrupted by a signal before any data was read; see signal(7).";
            break;
        case EINVAL:
            msg = "fd  is  attached to an object which is unsuitable for reading; or the file was opened with the O_DIRECT flag, and either the address specified in buf, the value specified in count, or the current file offset is not suitably aligned.\nfd was created via a call to timerfd_create(2) and the wrong size buffer was given to read(); see timerfd_create(2) for further information.";
            break;
        case EIO:
            msg = "I/O error.  This will happen for example when the process is in a background process group, tries to read from its controlling tty, and either it is ignoring or blocking SIGTTIN or its process group is orphaned.  It may also occur when there is a low-level I/O error while reading from a disk or tape.";
            break;
        case EISDIR:
            msg = "fd refers to a directory.";
            break;
        }
    #endif
    fprintf( stderr, "read() error #%d: %s\n", error, msg );
#endif
}

void error_write(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EAGAIN:
            msg = "The file descriptor fd refers to a file other than a socket and has been marked non-blocking (O_NONBLOCK), and the write would block.\nThe file descriptor fd refers to a socket and has been marked non-blocking (O_NONBLOCK), and the write would block. POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.";
            break;
        case EBADF:
            msg = "fd is not a valid file descriptor or is not open for writing.";
            break;
        case EFAULT:
            msg = "buf is outside your accessible address space.";
            break;
        case EFBIG:
            msg = "An attempt was made to write a file that exceeds the implementation-defined maximum file size or the process's file size limit, or to write at a position past the maximum allowed offset.";
            break;
        case EINTR:
            msg = "The call was interrupted by a signal before any data was written; see signal(7).";
            break;
        case EINVAL:
            msg = "fd is attached to an object which is unsuitable for writing; or the file was opened with the O_DIRECT flag, and either the address specified in buf, the value specified in count, or the current file offset is not suitably aligned.";
            break;
        case EIO:
            msg = "A low-level I/O error occurred while modifying the inode.";
            break;
        case ENOSPC:
            msg = "The device containing the file referred to by fd has no room for the data.";
            break;
        case EPIPE:
            msg = "fd is connected to a pipe or socket whose reading end is closed. When this happens the writing process will also receive a SIGPIPE signal. (Thus, the write return value is seen only if the program catches, blocks or ignores this signal.)";
            break;
        }
    fprintf( stderr, "write() error #%d: %s\n", error, msg );
    #endif
#endif
}

void error_recv(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EAGAIN:
            msg = "The socket is marked non-blocking and the receive operation would block, or a receive timeout had been set and the timeout expired before data was received. POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.";
            break;
        case EBADF:
            msg = "The argument sockfd is an invalid descriptor.";
            break;
        case ECONNREFUSED:
            msg = "A remote host refused to allow the network connection (typically because it is not running the requested service).";
            break;
        case EFAULT:
            msg = "The receive buffer pointer(s) point outside the process's address space.";
            break;
        case EINTR:
            msg = "The receive was interrupted by delivery of a signal before any data were available; see signal(7).";
            break;
        case EINVAL:
            msg = "Invalid argument passed.";
            break;
        case ENOMEM:
            msg = "Could not allocate memory for recvmsg().";
            break;
        case ENOTCONN:
            msg = "The socket is associated with a connection-oriented protocol and has not been connected (see connect(2) and accept(2)).";
            break;
        case ENOTSOCK:
            msg = "The argument sockfd does not refer to a socket.";
            break;
        }
    #endif
    fprintf( stderr, "recv() error #%d: %s\n", error, msg );
#endif
}

void error_send(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EACCES:
            msg = "(For  Unix  domain  sockets, which are identified by pathname) Write permission is denied on the destination socket file, or search permission is denied for one of the directories the path prefix.  (See path_resolution(7).)";
            break;
        case EAGAIN:
            msg = "The socket is marked non-blocking and the requested operation would block. POSIX.1-2001 allows either error to be returned for this case, and does not require these constants to have the same value, so a portable application should check for both possibilities.";
            break;
        case EBADF:
            msg = "An invalid descriptor was specified.";
            break;
        case ECONNRESET:
            msg = "Connection reset by peer.";
            break;
        case EDESTADDRREQ:
            msg = "The socket is not connection-mode, and no peer address is set.";
            break;
        case EFAULT:
            msg = "An invalid user space address was specified for an argument.";
            break;
        case EINTR:
            msg = "A signal occurred before any data was transmitted; see signal(7).";
            break;
        case EINVAL:
            msg = "Invalid argument passed.";
            break;
        case EISCONN:
            msg = "The connection-mode socket was connected already but a recipient was specified.  (Now either this error is returned, or the recipient specification is ignored.)";
            break;
        case EMSGSIZE:
            msg = "The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible.";
            break;
        case ENOBUFS:
            msg = "The output queue for a network interface was full. This generally indicates that the interface has stopped sending, but may be caused by transient congestion. (Normally, this does not occur in Linux.  Packets are just silently dropped when a device queue overflows.)";
            break;
        case ENOMEM:
            msg = "No memory available.";
            break;
        case ENOTCONN:
            msg = "The socket is not connected, and no target has been given.";
            break;
        case ENOTSOCK:
            msg = "The argument sockfd is not a socket.";
            break;
        case EOPNOTSUPP:
            msg = "Some bit in the flags argument is inappropriate for the socket type.";
            break;
        case EPIPE:
            msg = "The local end has been shut down on a connection oriented socket.  In this case the process will also receive a SIGPIPE unless MSG_NOSIGNAL is set.";
            break;
        }
    #endif
    fprintf( stderr, "send() error #%d: %s\n", error, msg );
#endif
}

void error_fork(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EAGAIN:
            msg = "fork() cannot allocate sufficient memory to copy the parent's page tables and allocate a task structure for the child.\nIt was not possible to create a new process because the caller's RLIMIT_NPROC resource limit was encountered. To exceed this limit, the process must have either the CAP_SYS_ADMIN or the CAP_SYS_RESOURCE capability.";
            break;
        case ENOMEM:
            msg = "fork() failed to allocate the necessary kernel structures because memory is tight.";
            break;
        }
    #endif
    fprintf( stderr, "fork() error #%d: %s\n", error, msg );
#endif
}

void error_exec(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
const char* msg = NULL;
#ifndef USEMINIMAL
    switch ( error )
        {
        case E2BIG:
            msg = "The total number of bytes in the environment (envp) and argument list (argv) is too large.";
            break;
        case EACCES:
            msg = "Search permission is denied on a component of the path prefix of filename or the name of a script interpreter.  (See also path_resolution(7).)\nThe file or a script interpreter is not a regular file.\nExecute permission is denied for the file or a script or ELF interpreter.\nThe file system is mounted noexec.";
            break;
        case EFAULT:
            msg = "filename points outside your accessible address space.";
            break;
        case EINVAL:
            msg = "An ELF executable had more than one PT_INTERP segment (i.e., tried to name more than one interpreter).";
            break;
        case EIO:
            msg = "An I/O error occurred.";
            break;
        case EISDIR:
            msg = "An ELF interpreter was a directory.";
            break;
#ifdef ELIBBAD
        case ELIBBAD:
            msg = "An ELF interpreter was not in a recognized format.";
            break;
#endif
        case ELOOP:
            msg = "Too many symbolic links were encountered in resolving filename or the name of a script or ELF interpreter.";
            break;
        case EMFILE:
            msg = "The process has the maximum number of files open.";
            break;
        case ENAMETOOLONG:
            msg = "filename is too long.";
            break;
        case ENFILE:
            msg = "The system limit on the total number of open files has been reached.";
            break;
        case ENOENT:
            msg = "The file filename or a script or ELF interpreter does not exist, or a shared library needed for file or interpreter cannot be found.";
            break;
        case ENOEXEC:
            msg = "An executable is not in a recognized format, is for the wrong architecture, or has some other format error that means it cannot be executed.";
            break;
        case ENOMEM:
            msg = "Insufficient kernel memory was available.";
            break;
        case ENOTDIR:
            msg = "A component of the path prefix of filename or a script or ELF interpreter is not a directory.";
            break;
        case EPERM:
            msg = "The file system is mounted nosuid, the user is not the superuser, and the file has the set-user-ID or set-group-ID bit set.\nThe process is being traced, the user is not the superuser and the file has the set-user-ID or set-group-ID bit set.";
            break;
        case ETXTBSY:
            msg = "Executable was open for writing by one or more processes.";
            break;
        }
    #endif
    fprintf( stderr, "exec() error #%d: %s\n", error, msg );
#endif
}

void error_fsync(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
const char* msg = NULL;
#ifndef USEMINIMAL
    switch ( error )
        {
        case EBADF:
            msg = "fd is not a valid file descriptor open for writing.";
            break;
        case EIO:
            msg = "An error occurred during synchronization.";
            break;
        case EINVAL:
            msg = "fd is bound to a special file which does not support synchronization.";
            break;
        }
    #endif
    fprintf( stderr, "fsync() error #%d: %s\n", error, msg );
#endif
}

void error_poll(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EFAULT:
            msg = "The array given as argument was not contained in the calling program's address space.";
            break;
        case EINTR:
            msg = "A signal occurred before any requested event; see signal(7).";
            break;
        case EINVAL:
            msg = "The nfds value exceeds the RLIMIT_NOFILE value.";
            break;
        case ENOMEM:
            msg = "There was no space to allocate file descriptor tables.";
            break;
        }
    #endif
    fprintf( stderr, "poll() error #%d: %s\n", error, msg );
#endif
}

void error_pipe(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EFAULT:
            msg = "pipefd is not valid.";
            break;
        case EINVAL:
            msg = "Invalid value in flags.";
            break;
        case EMFILE:
            msg = "Too many file descriptors are in use by the process.";
            break;
        case ENFILE:
            msg = "The system limit on the total number of open files has been reached.";
            break;
        }
    #endif
    fprintf( stderr, "pipe() error #%d: %s\n", error, msg );
#endif
}

void error_fcntl(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EACCES:
        case EAGAIN:
            msg = "Operation is prohibited by locks held by other processes. The operation is prohibited because the file has been memory-mapped by another process.";
            break;
        case EBADF:
            msg = "fd is not an open file descriptor, or the command was F_SETLK or F_SETLKW and the file descriptor open mode doesn't match with the type of lock requested.";
            break;
        case EDEADLK:
            msg = "It was detected that the specified F_SETLKW command would cause a deadlock.";
            break;
        case EFAULT:
            msg = "lock is outside your accessible address space.";
            break;
        case EINTR:
            msg = "For F_SETLKW, the command was interrupted by a signal; see signal(7). For F_GETLK and F_SETLK, the command was interrupted by a signal before the lock was checked or acquired. Most likely when locking a remote file (e.g., locking over NFS), but can sometimes happen locally.";
            break;
        case EINVAL:
            msg = "For F_DUPFD, arg is negative or is greater than the maximum allowable value.  For F_SETSIG, arg is not an allowable signal number.";
            break;
        case EMFILE:
            msg = "For F_DUPFD, the process already has the maximum number of file descriptors open.";
            break;
        case ENOLCK:
            msg = "Too many segment locks open, lock table is full, or a remote locking protocol failed (e.g., locking over NFS).";
            break;
        case EPERM:
            msg = "Attempted to clear the O_APPEND flag on a file that has the append-only attribute set.";
            break;
        }
    #endif
    fprintf( stderr, "fcntl() error #%d: %s\n", error, msg );
#endif
}

void error_getitimer(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        case EFAULT:
            msg = "new_value, old_value, or curr_value is not valid a pointer.";
            break;
        case EINVAL:
            msg = "which is not one of ITIMER_REAL, ITIMER_VIRTUAL, or ITIMER_PROF; or (since Linux 2.6.22) one of the tv_usec fields in the structure pointed to by new_value contains a value outside the range 0 to 999999.";
            break;
        }
    #endif
    fprintf( stderr, "error #%d: %s\n", error, msg );
#endif
}

void error_setitimer(int error)
{
error_getitimer( error );
}

/*
void error_(int error)
{
#ifdef USEPERROR
    errno = error;
    perror( NULL );
#else
    const char* msg = NULL;
    #ifndef USEMINIMAL
    switch ( error )
        {
        }
    #endif
    fprintf( stderr, "error: %s\n", msg );
#endif
}
*/
