#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <signal.h>

#include "netio.h"
#include "error.h"
#include "help.h"
#include "params.h"
#include "net.h"
#include "set.h"
#include "mainloop.h"

int g_ipid;
int g_opid;

int main(int argc, char* argv[])
{
if ( argc < 2 )
    {
    help();
    exit( EXIT_FAILURE );
    }

signal( SIGINT,  sigquit );
signal( SIGQUIT, sigquit );

params( argc, argv );

g_set = malloc( sizeof(struct pollfd) * 2 );
g_set[0].fd = STDOUT_FILENO;
g_set[0].events = 0;
g_set[0].revents = 0;
g_set[1].fd = STDIN_FILENO;
g_set[1].events = (p_ioevery || p_inevery)? 0 : POLLIN;
g_set[1].revents = 0;
g_setcount = 2;

if ( p_iocmd && !p_ioevery )
    {
    int newstdout[2];
    int newstdin[2];
    //TODO:
    pipe( newstdout );
    pipe( newstdin );
    g_ipid = run( p_iocmd, newstdout[1], newstdin[0] );
    g_opid = g_ipid;
    if ( g_ipid == EXIT_FAILURE )
        exit( EXIT_FAILURE );
    int ret = dup2( newstdout[0], STDIN_FILENO );
    if ( ret == -1 )
        {
        error_dup2( errno );
        exit( EXIT_FAILURE );
        }
    ret = dup2( newstdin[1], STDOUT_FILENO );
    if ( ret == -1 )
        {
        error_dup2( errno );
        exit( EXIT_FAILURE );
        }
    }
else
    {
    if ( p_incmd && !p_inevery )
        {
        int newstdout[2];
	//TODO:
        pipe( newstdout );
        g_ipid = run( p_incmd, newstdout[1], STDIN_FILENO );
        if ( g_ipid == EXIT_FAILURE )
            exit( EXIT_FAILURE );
        int ret = dup2( newstdout[0], STDIN_FILENO );
        if ( ret == -1 )
            {
            error_dup2( errno );
            exit( EXIT_FAILURE );
            }
        }
    if ( p_outcmd && !p_outevery )
        {
        int newstdin[2];
	//TODO:
        pipe( newstdin );
        g_opid = run( p_outcmd, STDOUT_FILENO, newstdin[0] );
        if ( g_opid == EXIT_FAILURE )
            exit( EXIT_FAILURE );
        int ret = dup2( newstdin[1], STDOUT_FILENO );
        if ( ret == -1 )
            {
            error_dup2( errno );
            exit( EXIT_FAILURE );
            }
        }
    }

int flags;
fcntl( STDOUT_FILENO, F_GETFD, &flags );
flags |= O_NONBLOCK;
if ( p_sync )
    {
    flags |= O_SYNC;
    }
fcntl( STDOUT_FILENO, F_SETFD, flags );

int index;
int proto;
if ( p_server )
    {
    for (index = 0; index < p_targetc; ++index)
        {
        int server_sock = mkserver( p_targetv[index], &proto );
        if ( net_params[proto].m_type == SOCK_STREAM )
            server_add( server_sock );
        else
            client_add( server_sock );
        }
    }
else
    {
    for (index = 0; index < p_targetc; ++index) 
        {
        int client_sock = mkclient( p_targetv[index], &proto );
        client_add( client_sock );
        }
    }
mainloop();

return 0;
};

void sigquit(int signal)
{
//TODO:
exit( EXIT_SUCCESS );
}

