#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#include "set.h"
#include "netio.h"

#define max(X,Y) (X>Y)?X:Y;

void add(struct TArray* array, int element)
{
register int count = array->m_count;
int* new = (int*)malloc( (count+1) * sizeof(int) );
if ( count > 0 )
    memcpy( new, array->m_array, count * sizeof(int) );
new[count] = element;
if ( count )
    free( array->m_array );
array->m_array = new;
++(array->m_count);
}

void del(struct TArray* array, int index)
{
register count = array->m_count;

if ( count == 1 )
    {
    free( array->m_array );
    array->m_array = NULL;
    array->m_count = 0;
    return;
    }

int* new = (int*)malloc( (count-1) * sizeof(int) );
if ( index > 0 )
    memcpy( new, array->m_array, index * sizeof(int) );
if ( (count - index) > 0 )
    memcpy( new + index, array->m_array + index + 1, (count - index) * sizeof(int) );
if ( count )
    free( array->m_array );
array->m_array = new;
--(array->m_count);
}

void mkfdset(fd_set* set)
{
FD_ZERO( set );

int index;
for (index = 0; index < s_fd.m_count; ++index)
    FD_SET( s_fd.m_array[index], set );
for (index = 0; index < c_fd.m_count; ++index)
    FD_SET( c_fd.m_array[index], set );
for (index = 0; index < i_fd.m_count; ++index)
    FD_SET( i_fd.m_array[index], set );
}

int mknfds()
{
int nfds = 0;
int index;
for (index = 0; index < s_fd.m_count; ++index)
    nfds = max( nfds, s_fd.m_array[index] );
for (index = 0; index < c_fd.m_count; ++index)
    nfds = max( nfds, c_fd.m_array[index] );
for (index = 0; index < i_fd.m_count; ++index)
    nfds = max( nfds, i_fd.m_array[index] );
++nfds;
return nfds;
}

