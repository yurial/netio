#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "netio.h"
#include "error.h"
#include "help.h"
#include "params.h"
#include "net.h"
#include "io.h"
#include "mainloop.h"
#include "signals.h"

int main(int argc, char* argv[])
{
if ( argc < 2 )
    {
    help();
    exit( EXIT_FAILURE );
    }

signals_init();

params( argc, argv );

io_init();

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
