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

if ( p_stdin && setstdin( p_stdin ) )
    exit( EXIT_FAILURE );
if ( p_stdout &&  setstdout( p_stdout ) )
    exit( EXIT_FAILURE );
if ( p_nonbuffering )
    nonbuffering();

int stdin_fd = STDIN_FILENO;

int* s_fd = NULL;
int* c_fd = NULL;
int* i_fd = malloc( sizeof(int) );
*i_fd = stdin_fd;

if ( p_server )
    {
    enum PROTO proto;
    int server_sock = mkserver( p_targetv[0], &proto );
    if ( net_params[proto].m_type == SOCK_STREAM )
        {
        s_fd = malloc( sizeof(int) );
	*s_fd = server_sock;
        mainloop( &s_fd, 1, &c_fd, 0, &i_fd, 1 );
	}
    else
        {
        c_fd = malloc( sizeof(int) );
	*c_fd = server_sock;
        mainloop( &s_fd, 0, &c_fd, 1, &i_fd, 1 );
        }
    }
else
    {
    enum PROTO proto;
    int client_sock = mkclient( p_targetv[0], &proto );
    c_fd = malloc( sizeof(int) );
    *c_fd = client_sock;
    mainloop( &s_fd, 0, &c_fd, 1, &i_fd, 1 );
    }

return 0;
};

void sigquit(int signal)
{
exit( EXIT_SUCCESS );
}

