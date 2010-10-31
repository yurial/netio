#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "servers.h"
#include "set.h"

#define MAX(X,Y) (X>Y)?X:Y
#define MIN(X,Y) (X<Y)?X:Y

struct TServers g_servers = { NULL, 0 };

void server_add(int sock)
{
int newcount = g_servers.m_count + 1;
struct TServer* servers = malloc( sizeof(struct TServer) * newcount );

memcpy( servers, g_servers.m_server, sizeof(struct TServer) * g_servers.m_count );
servers[g_servers.m_count].m_sock = sock;

set_add( 2 + g_servers.m_count , sock, POLLIN );

free( g_servers.m_server );
g_servers.m_server = servers;
g_servers.m_count = newcount;
}

void server_del(int sock)
{
int newcount = g_servers.m_count + 1;
struct TServer* servers = malloc( sizeof(struct TServer) * newcount );

int index;
for (index = 0; g_servers.m_server[index].m_sock != sock && index < g_servers.m_count; ++index)
    servers[index] = g_servers.m_server[index];

set_del( 2 + index );

for (++index; index < g_servers.m_count; ++index)
    servers[index-1] = g_servers.m_server[index];

free( g_servers.m_server );
g_servers.m_server = servers;
g_servers.m_count = newcount;
}

inline int is_server(int index)
{
return (index < (2 + g_servers.m_count)) && (index >= 2);
}

inline int servers_loop(int nready)
{
int index;
for (index = 2; nready > 0 && is_server( index ); ++index)
    {
    /* check for events */
    if ( g_set[index].revents )
        {
        --nready;
        }
    else
        {
        continue;
        }
    /**/
    assert( g_set[index].revents & POLLIN );
    /* accept new connection */
    int client_sock = accept( g_set[index].fd, NULL, NULL );
    if ( client_sock == -1 )
        {
        error_accept( errno );
        exit( EXIT_FAILURE );
        }
    client_add( client_sock );
    }
return nready;
}
