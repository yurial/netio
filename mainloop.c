#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "netio.h"
#include "mainloop.h"
#include "params.h"
#include "set.h"
#include "error.h"

void onconnect(int client_sock)
{
/*once*/
if ( p_once )
    {
    int index;
    for (index = 0; index < s_fd.m_count; ++index)
        del( &s_fd, index );
    }

int in = STDIN_FILENO;
int out = STDOUT_FILENO;
if ( p_ioevery )
    {
    int ret = run( p_iocmd, &in, &out );
    if  ( ret == EXIT_FAILURE )
        {
        close( client_sock );
        return;
        }
    }
else
    {
    if ( p_inevery )
        {
	int ret = run( p_incmd, &in, NULL );
	if  ( ret == EXIT_FAILURE )
	    {
	    close( client_sock );
	    return;
	    }
	}
    if ( p_outevery )
        {
	int ret = run( p_outcmd, NULL, &out );
	if  ( ret == EXIT_FAILURE )
	    {
	    close( client_sock );
	    return;
	    }
	}
    }

add( &i_fd, in );
add( &o_fd, out );
add( &c_fd, client_sock );
add( &c_ttl, -1 );
}

void disconnect(int index)
{
close( c_fd.m_array[index] );
if ( p_outevery || p_ioevery )
    close( o_fd.m_array[index] );
del( &c_fd, index );
del( &c_ttl, index );
del( &i_fd, index );
del( &o_fd, index );
}

int tvtoms(struct timeval tv)
{
return tv.tv_usec / 1000 + tv.tv_sec * 1000;
}

struct timeval mstotv(int ms)
{
struct timeval tv;
tv.tv_usec = (ms % 1000) * 1000;
tv.tv_sec = ms / 1000;
return tv;
}

void mainloop()
{
int ret;
/*p_buffsize*/
char* buff = (char*)malloc( p_buffsize );
if ( buff == NULL )
    {
    error_malloc( errno );
    exit( EXIT_FAILURE );
    }

fd_set set;
struct timeval  timeout;
struct timeval* ptimeout = NULL;

while ( (s_fd.m_count | c_fd.m_count) != 0 )
{
int index;

/*calc new timeout*/
int mstimeout = 1<<31 - 1;
for (index = 0; index < c_ttl.m_count; ++index)
    if ( c_ttl.m_array[index] > 0 && c_ttl.m_array[index] < mstimeout )
        mstimeout = c_ttl.m_array[index];
timeout = mstotv( mstimeout );
/**/

mkfdset( &set );
int nfds = mknfds();
int nready = select( nfds, &set, NULL, NULL, ptimeout );
int bbreak = 0;
/*calc new ttl*/
if ( ptimeout )
    {
    int passedtime = mstimeout - tvtoms( timeout );

    for (index = 0; index < c_ttl.m_count; ++index)
        {
        if ( c_ttl.m_array[ index ] > 0 )
            c_ttl.m_array[ index ] -= passedtime;
        if ( c_ttl.m_array[ index ] == 0 )
            {
	    disconnect( index );
	    --index; // after dissconnect count of elements decreminate
	    }
        }

    for (index = 0; index < c_ttl.m_count; ++index)
        if ( c_ttl.m_array[index] != -1 )
            bbreak = 1;
    if ( bbreak == 0 )
        ptimeout = NULL;
    bbreak = 0;
    }
/**/

if ( nready == 0 )
    continue;

if ( nready == -1 )
    {
    fprintf( stderr, "select() error\n" );
    exit( EXIT_FAILURE );
    }

for (index = 0; nready > 0 && index < s_fd.m_count; ++index)
    {
    if ( s_fd.m_array[index] >= 0 && FD_ISSET( s_fd.m_array[index], &set ) )
        {
	bbreak = 1;
	int client_sock = accept( s_fd.m_array[index], NULL, NULL );
	if ( client_sock == -1 )
	    {
	    fprintf( stderr, "accept() error\n" );
	    exit( EXIT_FAILURE );
	    }
	onconnect( client_sock );
	}
    }
if ( bbreak > 0 )
    continue;

for (index = 0; nready > 0 && index < c_fd.m_count; ++index)
    {
    if ( c_fd.m_array[index] >= 0 && FD_ISSET( c_fd.m_array[index], &set ) )
        {
	bbreak = 1;
	int recvcount = recv( c_fd.m_array[index], buff, p_buffsize, 0 );
        if ( recvcount == -1 || recvcount == 0 )
	    {
	    disconnect( index );
	    break;
	    }
        ret = write( o_fd.m_array[index], buff, recvcount );
	//TODO:
	assert( ret != -1 );
	assert( ret != 0 );
	/*sync*/
	if ( p_sync )
	  fsync( o_fd.m_array[index] );
	}
    }
if ( bbreak > 0 )
    continue;

for (index = 0; nready > 0 && index < i_fd.m_count; ++index)
    {
    if ( i_fd.m_array[index] >= 0 && FD_ISSET( i_fd.m_array[index], &set ) )
        {
        int readcount = read( i_fd.m_array[index], buff, p_buffsize  );
        if ( readcount == 0 || readcount == -1 )
	    {
	    if ( p_wait == 0 )
	        {
	    	disconnect( index );
		break;
		}
	    ptimeout = (p_wait == -1)? NULL : &timeout;
	    close( i_fd.m_array[index] );
	    index = (p_inevery || p_ioevery)? index-1 : -1;
            do
	        {
		++index;
                i_fd.m_array[index] = -1;
	        c_ttl.m_array[index] = p_wait;
		}
	    while (nready > 0 && index < c_fd.m_count-1 && !(p_inevery || p_ioevery));
	    continue;
	    }
	
	int sendcount;
	index = (p_inevery || p_ioevery)? index-1 : -1;
	do
	    {
	    ++index;
	    sendcount = send( c_fd.m_array[index], buff, readcount, 0 );
            if ( sendcount == -1 || sendcount == 0 )
                {
	        disconnect( index );
	        }
	    }
        while (nready > 0 && index < c_fd.m_count-1 && !(p_inevery || p_ioevery));
	}
    }
}

free( buff );
}
