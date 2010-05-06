#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#include "netio.h"

#define max(X,Y) (X>Y)?X:Y;

int* add(int* fds, int count, int element)
{
int* new = (int*)malloc( (count+1) * sizeof(int) );
if ( count > 0 )
    memcpy( new, fds, count * sizeof(int) );
new[count] = element;
if ( count )
    free( fds );
return new;
}

int* del(int* fds, int count, int index)
{
if ( (count-1) == 0 )
    {
    free( fds );
    return NULL;
    }

int* new = (int*)malloc( (count-1) * sizeof(int) );
if ( index > 0 )
    memcpy( new, fds, index * sizeof(int) );
if ( (count - index) > 0 )
    memcpy( new + index, fds + index + 1, (count - index) * sizeof(int) );
if ( count )
    free( fds );
return new;
}

void mkfdset(fd_set* set)
{
FD_ZERO( set );

int index;
for (index = 0; index < s_count; ++index)
    FD_SET( s_fd[index], set );
for (index = 0; index < c_count; ++index)
    FD_SET( c_fd[index], set );
for (index = 0; index < i_count; ++index)
    FD_SET( i_fd[index], set );
}

int mknfds()
{
int nfds = 0;
int index;
for (index = 0; index < s_count; ++index)
    nfds = max( nfds, s_fd[index] );
for (index = 0; index < c_count; ++index)
    nfds = max( nfds, c_fd[index] );
for (index = 0; index < i_count; ++index)
    nfds = max( nfds, i_fd[index] );
++nfds;
return nfds;
}

