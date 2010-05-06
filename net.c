#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "net.h"
#include "cp.h"

enum PROTO parsetarget(char* target, struct hostent** host, int* port)
{
enum PROTO proto = -1;
int index = 0;
while ( net_params[index].m_str )
    {
    if ( !memcmp( target, net_params[index].m_str, net_params[index].m_strlength ) )
         {
	 proto = net_params[index].m_proto;
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
    if ( proto == PROTO_UNIX )
        {
	*host = malloc( sizeof(struct hostent) );
	memset( *host, 0, sizeof(struct hostent) );
        (*host)->h_length = strlen( target );
	(*host)->h_addr_list = malloc( sizeof(char*) );
	*((*host)->h_addr_list) = cp( target );
        return proto;
	}
    else
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
    {
    *port = serv->s_port;
    }
else
    {
    *port = htons( atoi( hostend ) );
    //TODO:
    }
return proto;
}

