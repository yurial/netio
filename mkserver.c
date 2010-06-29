#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netdb.h>

#include "params.h"
#include "mkserver.h"
#include "net.h"
#include "cp.h"

int mkserver(char* target, int* pproto)
{
int server_addr_size = 0;
struct sockaddr*    server_addr;
struct sockaddr_un  server_addr_un;
struct sockaddr_in  server_addr_in;
struct sockaddr_in6 server_addr_in6;
memset( &server_addr_un, 0, sizeof(server_addr_un) );
memset( &server_addr_in, 0, sizeof(server_addr_in) );
memset( &server_addr_in6, 0, sizeof(server_addr_in6) );

struct hostent* host;
int port = 0;

int proto = parsetarget( target, &host, &port );
int server_sock = socket( net_params[proto].m_domain, net_params[proto].m_type, net_params[proto].m_protocol );
if ( server_sock == -1 )
    {
    fprintf( stderr, "socket() error\n" );
    exit( EXIT_FAILURE );
    }

if ( net_params[proto].m_domain == AF_UNIX )
    {
    server_addr_un.sun_family = AF_UNIX;
    strncpy( server_addr_un.sun_path, host->h_addr, host->h_length+1 );
    server_addr = (struct sockaddr*)&server_addr_un;
    server_addr_size = sizeof(server_addr_un);
    }
else if ( net_params[proto].m_domain == AF_INET )
    {
    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_port = port;
    assert( sizeof(server_addr_in.sin_addr.s_addr) == host->h_length );
    memcpy( &server_addr_in.sin_addr.s_addr, host->h_addr, host->h_length );
    server_addr = (struct sockaddr*)&server_addr_in;
    server_addr_size = sizeof(server_addr_in);
    }
else if ( net_params[proto].m_domain == AF_INET6 )
    {
    server_addr_in6.sin6_family = AF_INET6;
    server_addr_in6.sin6_port = port;
    assert( sizeof(server_addr_in6.sin6_addr) == host->h_length );
    memcpy( &server_addr_in6.sin6_addr, host->h_addr, host->h_length );
    server_addr = (struct sockaddr*)&server_addr_in6;
    server_addr_size = sizeof(server_addr_in6);
    }

int ret = bind( server_sock, server_addr, server_addr_size );
if ( ret != 0 )
    {
    fprintf( stderr, "bind() error\n" );
    exit( EXIT_FAILURE );
    }

if ( net_params[proto].m_type == SOCK_STREAM )
    {
    ret = listen( server_sock, p_connqueue );
    if ( ret != 0 )
        {
        fprintf( stderr, "listen() error\n" );
        exit( EXIT_FAILURE );
        }
    }

*pproto = proto;
return server_sock;
}

