#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <signal.h>

#include "netio.h"
#include "help.h"
#include "params.h"
#include "net.h"
#include "terminal.h"
#include "set.h"

void sigquit(int signal);

struct TArray s_fd  = { NULL, 0 };
struct TArray c_fd  = { NULL, 0 };
struct TArray c_ttl = { NULL, 0 };
struct TArray i_fd  = { NULL, 0 };
struct TArray o_fd  = { NULL, 0 };

int main(int argc, char* argv[])
{
GetStartTerminal();
atexit( SetStartTerminal );

if ( argc < 2 )
    {
    help();
    exit( EXIT_FAILURE );
    }

signal( SIGINT,  sigquit );
signal( SIGQUIT, sigquit );

params( argc, argv );
if ( p_iocmd && !p_ioevery )
    {
    int i_fd, o_fd;
    int ret = run( p_iocmd, &i_fd, &o_fd );
    if ( ret == EXIT_FAILURE )
        exit( EXIT_FAILURE );
    dup2( i_fd, STDIN_FILENO );
    dup2( o_fd, STDOUT_FILENO );
    }
else
    {
    if ( p_incmd && !p_inevery )
        {
	int i_fd;
	int ret = run( p_incmd, &i_fd, NULL );
	if ( ret == EXIT_FAILURE )
	    exit( EXIT_FAILURE );
	dup2( i_fd, STDIN_FILENO );
	}
    if ( p_outcmd && !p_outevery )
        {
	int o_fd;
	int ret = run( p_outcmd, NULL, &o_fd );
	if ( ret == EXIT_FAILURE )
	    exit( EXIT_FAILURE );
	dup2( o_fd, STDOUT_FILENO );
	}
    }

if ( p_server )
    {
    enum PROTO proto;
    int server_sock = mkserver( p_targetv[0], &proto );
    if ( net_params[proto].m_type == SOCK_STREAM )
        {
        add( &s_fd, server_sock );
        mainloop();
	}
    else
        {
        add( &c_fd, server_sock );
        mainloop();
        }
    }
else
    {
    enum PROTO proto;
    int client_sock = mkclient( p_targetv[0], &proto );
    add( &c_fd, client_sock );
    mainloop();
    }

return 0;
};

void sigquit(int signal)
{
exit( EXIT_SUCCESS );
}

