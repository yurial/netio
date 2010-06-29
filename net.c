#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "net.h"
#include "cp.h"

const struct net_param net_params[] = {
#ifdef USEUNIX  
	{ STR_UNIX,  sizeof(STR_UNIX) -1, STR_NAME_UNIX, AF_UNIX,  SOCK_STREAM, IPPROTO_IP   }, 
#endif
#ifdef USETCP4  
	{ STR_TCP4,  sizeof(STR_TCP4) -1, STR_NAME_TCP, AF_INET,  SOCK_STREAM,  IPPROTO_TCP  }, 
#endif
#ifdef USEUDP4  
	{ STR_UDP4,  sizeof(STR_UDP4) -1, STR_NAME_UDP, AF_INET,  SOCK_DGRAM,   IPPROTO_UDP  }, 
#endif
#ifdef USESCTP4 
	{ STR_SCTP4, sizeof(STR_SCTP4)-1, STR_NAME_SCTP, AF_INET,  SOCK_STREAM, IPPROTO_SCTP }, 
#endif
#ifdef USETCP6  
	{ STR_TCP6,  sizeof(STR_TCP6) -1, STR_NAME_TCP, AF_INET6, SOCK_STREAM,  IPPROTO_TCP  },
#endif
#ifdef USEUDP6  
	{ STR_UDP6,  sizeof(STR_UDP6) -1, STR_NAME_UDP, AF_INET6, SOCK_DGRAM,   IPPROTO_UDP  }, 
#endif
#ifdef USESCTP6 
	{ STR_SCTP6, sizeof(STR_SCTP6)-1, STR_NAME_SCTP, AF_INET6, SOCK_STREAM, IPPROTO_SCTP }, 
#endif
        { NULL,      0,                   NULL,          -1,       -1,          -1 }
};

int parsetarget(char* target, struct hostent** host, int* port)
{
int proto = -1;
int index = 0;
while ( net_params[index].m_str )
    {
    if ( !memcmp( target, net_params[index].m_str, net_params[index].m_strlength ) )
         {
	 proto = index;
	 target += net_params[index].m_strlength;
	 break;
	 }
    ++index;
    }
if ( proto == -1 )
    {
    fprintf( stderr, "protocol not specified\n" );
    exit( EXIT_FAILURE );
    }

char endchar = ( *target == '[' )? ']' : ':';

char* hostend;
for (hostend = target; *hostend && *hostend != endchar; ++hostend)
    ;


if ( !*hostend )
#ifdef USEUNIX
	if ( net_params[index].m_str == STR_UNIX )
        {
	*host = malloc( sizeof(struct hostent) );
	memset( *host, 0, sizeof(struct hostent) );
        (*host)->h_length = strlen( target );
	(*host)->h_addr_list = malloc( sizeof(char*) );
	*((*host)->h_addr_list) = cp( target );
        return proto;
	}
    else
#endif
        {
        fprintf( stderr, "port not specified\n" );
        exit( EXIT_FAILURE );
        }

if ( *hostend == ']' )
    ++hostend;

int lenght = hostend - target;
char* hostname= malloc( lenght + 1 );
memcpy( hostname, target, lenght );
hostname[lenght] = 0;
*host = gethostbyname2( hostname, net_params[index].m_domain );
free( hostname );

++hostend;
struct servent* serv = getservbyname( hostend, net_params[index].m_protoname );
if ( serv )
    *port = serv->s_port;
else
    *port = htons( atoi( hostend ) );
return proto;
}

