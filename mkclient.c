#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/un.h>
#include <sys/types.h>

#include "error.h"
#include "params.h"
#include "mkclient.h"
#include "net.h"

int mkclient(char* target, int* pproto)
{
int ret;
int client_addr_size = 0;
struct sockaddr*    client_addr;
struct sockaddr_un  client_addr_un;
struct sockaddr_in  client_addr_in;
struct sockaddr_in6 client_addr_in6;
memset( &client_addr_un, 0, sizeof(client_addr_un) );
memset( &client_addr_in, 0, sizeof(client_addr_in) );
memset( &client_addr_in6, 0, sizeof(client_addr_in6) );

struct hostent* host;
int port = 0;

int proto = parsetarget( target, &host, &port );
if ( host == NULL )
    {
    fprintf( stderr, "invalid host\n" );
    exit( EXIT_FAILURE );
    }

int client_sock = socket( net_params[proto].m_domain, net_params[proto].m_type, net_params[proto].m_protocol );
if ( client_sock == -1 )
    {
    error_socket( errno );
    exit( EXIT_FAILURE );
    }

if ( net_params[proto].m_type == SOCK_DGRAM )
    {
    unsigned long value = (unsigned long)-1;
    ret = setsockopt( client_sock, SOL_SOCKET, SO_BROADCAST, &value, sizeof(value) );
    if ( ret != 0 )
        {
        fprintf( stderr, "setsockopt error!\n" );
        exit( EXIT_FAILURE );
        }
    }

if ( net_params[proto].m_domain == AF_UNIX )
    {
    client_addr_un.sun_family = AF_UNIX;
    strncpy( client_addr_un.sun_path, host->h_addr, host->h_length+1 );
    client_addr = (struct sockaddr*)&client_addr_un;
    client_addr_size = sizeof(client_addr_un);
    }
else if ( net_params[proto].m_domain == AF_INET )
    {
    client_addr_in.sin_family = AF_INET;
    client_addr_in.sin_port = port;
    assert( sizeof(client_addr_in.sin_addr.s_addr) == host->h_length );
    memcpy( &client_addr_in.sin_addr.s_addr, host->h_addr, host->h_length );
    client_addr = (struct sockaddr*)&client_addr_in;
    client_addr_size = sizeof(client_addr_in);
    }
else if ( net_params[proto].m_domain == AF_INET6 )
    {
    client_addr_in6.sin6_family = AF_INET6;
    client_addr_in6.sin6_port = port;
    assert( sizeof(client_addr_in6.sin6_addr) == host->h_length );
    memcpy( &client_addr_in6.sin6_addr, host->h_addr, host->h_length );
    client_addr = (struct sockaddr*)&client_addr_in6;
    client_addr_size = sizeof(client_addr_in6);
    }

ret = connect( client_sock, client_addr, client_addr_size );
if ( ret != 0 )
    {
    error_connect( errno );
    exit( EXIT_FAILURE );
    }

*pproto = proto;
return client_sock;
}

