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

void mainloop()
{
int ret;
/*p_buffsize*/
char* buff = (char*)malloc( p_buffsize );

fd_set set;
struct timeval  timeout;
struct timeval* ptimeout = NULL;
timeout.tv_usec = 0;
timeout.tv_sec = 0;

/*wait*/
if ( p_wait )
    timeout.tv_sec = p_wait;

while ( (s_count + c_count) != 0 )
{
mkfdset( &set );
int nfds = mknfds();
int nready = select( nfds, &set, NULL, NULL, ptimeout );
if ( nready == 0 )
    break; //timeout
if ( nready == -1 )
    {
    fprintf( stderr, "select() error\n" );
    exit( EXIT_FAILURE );
    }
int bbreak = 0;
int index;
for (index = 0; nready > 0 && index < s_count; ++index)
    {
    if ( FD_ISSET( s_fd[index], &set ) )
        {
	bbreak = 1;
	int client_sock = accept( s_fd[index], NULL, NULL );
	if ( client_sock == -1 )
	    {
	    fprintf( stderr, "accept() error\n" );
	    exit( EXIT_FAILURE );
	    }
	/*once*/
	if ( p_once )
	    {
	    int i;
	    for (i = 0; i < s_count; ++i)
	        close( s_fd[i] );
            free( s_fd );
	    s_fd = NULL;
	    s_count = 0;
	    }
	/*onconnect*/
	if ( p_exec )
	    system( p_exec );
	/*credirect*/
	if ( p_credirect )
	    {
	    int in;
	    int out;
	    int ret = run( p_credirect, &in, &out );
	    if ( ret == EXIT_FAILURE )
	        {
		close( client_sock );
		continue;
		}
	    i_fd = add( i_fd, i_count, in );
	    ++i_count;
	    o_fd = add( o_fd, c_count, out );
	    ++o_count;
	    }
	
	c_fd = add( c_fd, c_count, client_sock );
	++c_count;
	}
    }
if ( bbreak > 0 )
    continue;
for (index = 0; nready > 0 && index < c_count; ++index)
    {
    if ( FD_ISSET( c_fd[index], &set ) )
        {
	bbreak = 1;
	int recvcount = recv( c_fd[index], buff, p_buffsize, 0 );
	if ( recvcount == -1 || recvcount == 0 )
	    {
	    close( c_fd[index] );
            c_fd = del( c_fd, c_count, index );
	    --c_count;
	    if ( p_credirect )
                {
		close( i_fd[index] );
		close( o_fd[index] );
		i_fd = del( i_fd, i_count, index );
		o_fd = del( o_fd, o_count, index );
	        --i_count;
		--o_count;
		}
	    continue;
	    }
	if ( p_credirect )
	    ret = write( o_fd[index], buff, recvcount );
	else
	    ret = write( STDOUT_FILENO, buff, recvcount );
	//TODO:
	assert( ret != -1 );
	assert( ret != 0 );
	/*sync*/
	if ( p_sync )
	    if ( p_credirect )
	        fsync( o_fd[index] );
            else
	        fsync( STDOUT_FILENO );
	}
    }
if ( bbreak > 0 )
    continue;
for (index = 0; nready > 0 && index < i_count; ++index)
    {
    if ( FD_ISSET( i_fd[index], &set ) )
        {
        int readcount = read( i_fd[index], buff, p_buffsize  );
        if ( readcount == 0 || readcount == -1 )
	    {
            if ( p_credirect )
	        {
                close( c_fd[index] );
		close( o_fd[index] );
		c_fd = del( c_fd, c_count, index );
		o_fd = del( o_fd, o_count, index );
                --c_count;
		--o_count;
		}
	    else
	        ptimeout = &timeout;
	    close( i_fd[index] );
	    i_fd = del( i_fd, i_count, index );
	    --i_count;
	    continue;
	    }
	
	int sendcount;
	if ( p_credirect )
	    {
	    sendcount = send( c_fd[index], buff, readcount, 0 );
            if ( sendcount == -1 || sendcount == 0 )
                {
                close( c_fd[index] );
	        c_fd = del( c_fd, c_count, index );
                --c_count;
	        }
	    }
	else
	    {
	    int sendindex;
	    for (sendindex = 0; sendindex < c_count; ++sendindex)
	        {
		sendcount = send( c_fd[sendindex], buff, readcount, 0 );
                if ( sendcount == -1 || sendcount == 0 )
                    {
	            close( c_fd[sendindex] );
	            c_fd = del( c_fd, c_count, sendindex );
                    --c_count;
	            }
		}
	    }
        }
    }
}
free( buff );
}
