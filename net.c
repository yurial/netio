#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "net.h"
#include "cp.h"

enum PROTO getproto(char* target)
{
if ( !memcmp( STR_UNIX, target, sizeof(STR_UNIX)-1 ) )
    return PROTO_UNIX;
if ( !memcmp( STR_TCP, target, sizeof(STR_TCP)-1 ) )
    return PROTO_TCP;
if ( !memcmp( STR_UDP, target, sizeof(STR_UDP)-1 ) )
    return PROTO_UDP;
if ( !memcmp( STR_SCTP, target, sizeof(STR_SCTP)-1 ) )
    return PROTO_SCTP;
return PROTO_UNKNOWN;
}

size_t getprotolenght(enum PROTO proto)
{
switch( proto )
    {
    case PROTO_UNKNOWN:
        return 0;
        break;
    case PROTO_TCP:
        return sizeof(STR_TCP)-1;
        break;
    case PROTO_UDP:
        return sizeof(STR_UDP)-1;
        break;
    case PROTO_SCTP:
        return sizeof(STR_SCTP)-1;
        break;
    case PROTO_UNIX:
        return sizeof(STR_UNIX)-1;
        break;
    default:
        assert( 0 );
        break;
    }
return -1;
}

char* gethost(enum PROTO proto, char* target)
{
target += getprotolenght( proto );

char endchar = ( *target == '[' )? ']' : ':';

char* hostend;
for (hostend = target; *hostend && *hostend != endchar; ++hostend)
    ;
if ( !*hostend )
    return cp( target );

if ( *hostend == ']' )
    ++hostend;

int lenght = hostend - target;
char* host = malloc( lenght + 1 );
memcpy( host, target, lenght );
host[lenght] = 0;
return host;
}

int getport(enum PROTO proto, char* host, char* target)
{
target += getprotolenght( proto ) + strlen( host ) + 1;
return atoi( target );
}
