#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "clients.h"
#include "servers.h"
#include "params.h"
#include "set.h"

#define MAX(X,Y) (X>Y)?X:Y
#define MIN(X,Y) (X<Y)?X:Y

struct TClients g_clients = { NULL, 0, 0 };

inline int set2client(int index)
{
return index - 2 - g_servers.m_count;
}

inline int client2set(int index)
{
return index + 2 + g_servers.m_count;
}

void client_add(int sock)
{
int events = POLLHUP;
struct TClient client;
client.m_sock = sock;
client.m_sendbuff = NULL;
client.m_remain = 0;

if ( p_ioevery )
    {
    client.m_opid = run( p_iocmd, sock, sock );
    client.m_ipid = client.m_opid;
    }
else
    {
    if ( p_inevery )
        {
        client.m_ipid = run( p_incmd, sock, STDIN_FILENO );
        }
    else
        {
        client.m_ipid = -1;
        }
    if ( p_outevery )
        {
        client.m_opid = run( p_outcmd, STDOUT_FILENO, sock );
        }
    else
        {
        client.m_opid = -1;
        events |= POLLIN;
        }
    }

//TODO:
int flags = fcntl( sock, F_GETFL, &flags );
flags |= O_NONBLOCK;
fcntl( sock, F_SETFL, flags );

int newcount = g_clients.m_count + 1;
struct TClient* clients = malloc( sizeof(struct TClient) * newcount );

memcpy( clients, g_clients.m_client, sizeof(struct TClient) * g_clients.m_count );
clients[ g_clients.m_count ] = client;

set_add( client2set( 0 ) + g_clients.m_count, sock, events );

free( g_clients.m_client );
g_clients.m_client = clients;
g_clients.m_count = newcount;
}

void client_del(int sock)
{
int newcount = g_clients.m_count - 1;
struct TClient* clients = malloc( sizeof(struct TClient) * newcount );

int index;
for (index = 0; g_clients.m_client[index].m_sock != sock && index < g_clients.m_count; ++index)
    {
    clients[index] = g_clients.m_client[index];
    }

close( sock );
if ( g_clients.m_client[index].m_remain != 0  )
    {
    --g_clients.m_blocked;
    }
if ( g_clients.m_client[index].m_ipid != -1 )
    {
    kill( g_clients.m_client[index].m_ipid, p_signal );
    }
if ( g_clients.m_client[index].m_opid != -1 )
    {
    kill( g_clients.m_client[index].m_opid, p_signal );
    }

set_del( client2set( 0 ) + index );

for (++index; index < g_clients.m_count; ++index)
    {
    clients[index-1] = g_clients.m_client[index];
    }

free( g_clients.m_client );
g_clients.m_client = clients;
g_clients.m_count = newcount;
}

inline int clients_loop(char* recvbuff, int nready)
{
int setindex = client2set( 0 );
int clientindex = 0;

while ( nready > 0 && clientindex < g_clients.m_count )
    {
    struct pollfd* set = g_set + setindex;
    struct TClient* client = g_clients.m_client + clientindex;

    /* check for events */
    if ( !set->revents )
        {
        ++clientindex;
        ++setindex;
        continue;
        }
    /* disconnect client */
    if ( set->revents & POLLHUP )
        {
        --nready;
        client_del( client->m_sock );
        continue;
        }
    /* can send to client */
    if ( set->revents & POLLOUT )
        {
        nready = client_POLLOUT( nready, set, client, recvbuff );
        continue;
        }
    /* read from client */
    if ( set->revents & POLLIN )
        {
        nready = client_POLLIN( nready, set, client, recvbuff );
        continue;
        }
    assert( 0 );
    }
if ( g_clients.m_blocked == 0 )
    {
    g_set[1].events |= POLLIN;
    }
return nready;
}

inline int client_POLLOUT(int nready, struct pollfd* set, struct TClient* client, char* recvbuff)
{
set->revents ^= POLLOUT;
--nready;

const int sock = client->m_sock;
const char* sendbuff = client->m_sendbuff;
const int remain = client->m_remain;

int sendsize = send( sock, sendbuff, remain, 0/*MSG_NOSIGNAL*/ );
if ( sendsize == -1 )
    {
    /* if error */
    error_send( errno );
    client_del( client->m_sock );
    return nready;
    }
if ( sendsize < remain )
    {
    /* if not all sended */
    client->m_sendbuff += sendsize;
    client->m_remain -= sendsize;
    }
else
    {
    /* if all sended */
    client->m_sendbuff = NULL;
    client->m_remain = 0;
    set->events &= ~POLLOUT;
    --g_clients.m_blocked;
    }
return nready;
}

inline int client_POLLIN(int nready, struct pollfd* set, struct TClient* client, char* recvbuff)
{
set->revents ^= POLLIN;
--nready;

ssize_t recvsize = recv( set->fd, recvbuff, p_buffsize, MSG_PEEK );
if ( recvsize == -1 )
    {
    error_recv( errno );
    client_del( client->m_sock );
    return nready;
    }
if ( recvsize == 0 )
    {
    client_del( client->m_sock );
    return nready;
    }
ssize_t writesize = write( STDOUT_FILENO, recvbuff, recvsize );
if ( writesize == -1 )
    {
    error_write( errno );
    //TODO:
    assert( 0 );
    }
ssize_t truncsize = recv( client->m_sock, NULL, writesize, MSG_TRUNC );
assert( truncsize == writesize ); //TODO: WTF?
if ( writesize < recvsize )
    {
    g_set[0].events |= POLLOUT;
    int clientindex = 0;
    set = g_set + (2 + g_servers.m_count);
    while ( nready > 0 && clientindex < g_clients.m_count )
        {
        if ( set->revents & POLLIN )
            {
            --nready;
            set->revents ^= POLLIN;
            }
        set->events &= ~POLLIN;
        ++set;
        ++clientindex;
        }
    }
return nready;
}

inline void client_sendall(char* buff, const size_t buffsize)
{
int clientindex = 0;
int setindex = client2set( 0 );
while (clientindex < g_clients.m_count)
    {
    struct pollfd* set = g_set + setindex;
    struct TClient* client = g_clients.m_client + clientindex;

    int sendsize = send( client->m_sock, buff, buffsize, 0/*MSG_NOSIGNAL*/ );
    if ( sendsize == -1 )
        {
        if ( errno == EWOULDBLOCK )
            {
            sendsize = 0;
            }
        else
            {
            error_send( errno );
            client_del( client->m_sock );
            continue;
            }
        }
    if ( sendsize < buffsize )
        {
        ++g_clients.m_blocked;
        client->m_sendbuff = buff + sendsize;
        client->m_remain = buffsize - sendsize;
        set->events |= POLLOUT;
        }
    ++clientindex;
    ++setindex;
    }
}

