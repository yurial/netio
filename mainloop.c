#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "mainloop.h"
#include "params.h"
#include "set.h"

void mainloop(int** s_fd, int s_count, int** c_fd, int c_count, int** i_fd, int i_count)
{
char* buff = (char*)malloc( p_buffsize );
fd_set set;
do
{
mkfdset( &set, *s_fd, s_count, *c_fd, c_count, *i_fd, i_count );
int nfds = mknfds( *s_fd, s_count, *c_fd, c_count, *i_fd, i_count );
int ret = select( nfds, &set, NULL, NULL, NULL );
assert( ret != 0 );
if ( ret == -1 )
    {
    fprintf( stderr, "select() error\n" );
    exit( EXIT_FAILURE );
    }
int bbreak = 0;
int index;
for (index = 0; index < s_count; ++index)
    {
    if ( FD_ISSET( (*s_fd)[index], &set ) )
        {
	bbreak = 1;
	int client_sock = accept( (*s_fd)[index], NULL, NULL );
	if ( client_sock == -1 )
	    {
	    fprintf( stderr, "accept() error\n" );
	    exit( EXIT_FAILURE );
	    }
	*c_fd = add( *c_fd, c_count, client_sock );
	++c_count;
	}
    }
if ( bbreak > 0 )
    continue;
for (index = 0; index < c_count; ++index)
    {
    if ( FD_ISSET( (*c_fd)[index], &set ) )
        {
	bbreak = 1;
	int recvcount = recv( (*c_fd)[index], buff, p_buffsize, 0 );
	if ( recvcount == -1 || recvcount == 0 )
	    {
	    close( (*c_fd)[index] );
            *c_fd = del( *c_fd, c_count, index );
	    --c_count;
	    continue;
	    }
	write( STDOUT_FILENO, buff, recvcount );
	if ( p_sync )
	    fsync( STDOUT_FILENO );
	//TODO: if error &>/dev/null
	}
    }
if ( bbreak > 0 )
    continue;
for (index = 0; index < i_count; ++index)
    {
    if ( FD_ISSET( (*i_fd)[index], &set ) )
        {
        int readcount = read( STDIN_FILENO, buff, p_buffsize  );
        //TODO: if error < FD_CLR;
	int sendindex;
	for (sendindex = 0; sendindex < c_count; ++sendindex)
	    {
	    int sendcount = send( (*c_fd)[sendindex], buff, readcount, 0 );
            if ( sendcount == -1 || sendcount == 0 )
	        {
		//TODO: FD_CLR
		}
	    }
	}
    }
}
while ( 1 ); //TODO mainloop
free( buff );
}

