#include <stdio.h>

void help()
{
fprintf( stderr, "%s\n", "netio -hl1qciowsb --io <target> <target> ..." );
fprintf( stderr, "%s\n", "<target>          <local>|<net>" );
fprintf( stderr, "%s\n", "<net>             [<proto>://]<host>:<port>" );
fprintf( stderr, "%s\n", "<proto>           tcp|udp|sctp" );
fprintf( stderr, "%s\n", "<host>            <ipv4>|<ipv6>|<host name>" );
fprintf( stderr, "%s\n", "<port>            single numeric 1-65535" );
fprintf( stderr, "%s\n", "<local>           unix:<socket>" );
fprintf( stderr, "%s\n", "<socket>          path to unix socket" );
}

void build_str(FILE* file)
{
fprintf( file, "v%s build %s %s\n", VERSION, BUILD, DATE );
}

