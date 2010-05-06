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

void sigquit(int signal);

int* s_fd = NULL;
int* c_fd = NULL;
int* i_fd = NULL;
int* o_fd = NULL;

int  s_count = 0;
int  c_count = 0;
int  i_count = 0;
int  o_count = 0;

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

if ( !p_credirect )
    {
    i_fd = malloc( sizeof(int) );
    o_fd = malloc( sizeof(int) );
    i_count = 1;
    o_count = 1;
    }
if ( p_predirect )
    {
    int ret = run( p_predirect, i_fd, o_fd );
    if ( ret == EXIT_FAILURE )
        exit( EXIT_FAILURE );
    dup2( *i_fd, STDIN_FILENO );
    dup2( *o_fd, STDOUT_FILENO );
    }
else if ( !p_credirect )
    {
    if ( p_stdin && setstdin( p_stdin ) )
        exit( EXIT_FAILURE );
    else if ( p_nonbuffering )
        nonbuffering();
    if ( p_stdout &&  setstdout( p_stdout ) )
        exit( EXIT_FAILURE );
    *i_fd = STDIN_FILENO;
    *o_fd = STDOUT_FILENO;
    }

if ( p_server )
    {
    enum PROTO proto;
    int server_sock = mkserver( p_targetv[0], &proto );
    if ( net_params[proto].m_type == SOCK_STREAM )
        {
        s_fd = malloc( sizeof(int) );
	s_count = 1;
	*s_fd = server_sock;
        mainloop();
	}
    else
        {
        c_fd = malloc( sizeof(int) );
	c_count = 1;
	*c_fd = server_sock;
        mainloop();
        }
    }
else
    {
    enum PROTO proto;
    int client_sock = mkclient( p_targetv[0], &proto );
    c_fd = malloc( sizeof(int) );
    c_count = 1;
    *c_fd = client_sock;
    mainloop();
    }

return 0;
};

void sigquit(int signal)
{
exit( EXIT_SUCCESS );
}

