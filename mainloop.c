#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "set.h"
#include "servers.h"
#include "clients.h"
#include "mainloop.h"
#include "params.h"

#include <signal.h>

void mainloop()
{
/*p_buffsize*/
char* recvbuff = (char*)malloc( p_recvbuff );
char* sendbuff = (char*)malloc( p_sendbuff );
if ( recvbuff == NULL || sendbuff == NULL )
    {
    error_malloc( errno );
    exit( EXIT_FAILURE );
    }
/**/

sigset_t normalsig;
sigprocmask( SIG_SETMASK, NULL, &normalsig );
sigset_t minimalsig;
sigemptyset ( &minimalsig );
sigaddset( &minimalsig, SIGILL );
sigaddset( &minimalsig, SIGFPE );
sigaddset( &minimalsig, SIGSEGV );
sigaddset( &minimalsig, SIGBUS );
sigaddset( &minimalsig, SIGPOLL );
//sigaddset( &minimalsig, );

while ( (g_servers.m_count | g_clients.m_count) != 0 )
    {
    sigprocmask( SIG_SETMASK, &normalsig, NULL );
    int nready = poll( g_set, g_setcount, -1 );
    sigprocmask( SIG_SETMASK, &minimalsig, NULL );
    if ( nready == -1 )
        {
        if ( errno == EINTR )
            {
	    fprintf( stderr, "eintr\n" );
	    continue;
	    }
        error_poll( errno );
        exit( EXIT_FAILURE );
        }
    if ( nready == 0 )
        continue;
    nready = output_loop( nready );
    if ( nready == 0 )
        continue;
    nready = input_loop( sendbuff, nready );
    if ( nready == 0 )
        continue;
    nready = servers_loop( nready );
    if ( nready == 0 )
        continue;
    nready = clients_loop( recvbuff, nready );
    assert( nready == 0 );
    }

free( sendbuff );
free( recvbuff );
}
