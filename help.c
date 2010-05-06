#include <stdio.h>
#include "examples.h"

void help()
{
fprintf( stderr, "%s\n\n", "NAME" );
fprintf( stderr, "%s\n", "netio - bind stdin & stdout with socket" );
fprintf( stderr, "\n%s\n\n", "SYNOPSIS" );
fprintf( stderr, "%s\n", "netio [-l <target> [-1]] [-q <count>] [-i <file>] [-o <file>] [-e <command>] [-p <command>] [-c <command>] [-w <sec>] [-a] [-s] [-n] [-b <size[K|M]>] <target>" );
fprintf( stderr, "%s\n", "<target>          <local>|<net>" );
fprintf( stderr, "%s\n", "<net>             [<proto>://]<host>:<port>, if <proto> not specified use tcp" );
fprintf( stderr, "%s\n", "<proto>           tcp|udp|sctp" );
fprintf( stderr, "%s\n", "<host>            <ipv4>|<ipv6>|<host name>" );
fprintf( stderr, "%s\n", "<port>            single numeric 1-65535" );
fprintf( stderr, "%s\n", "<local>           unix:<socket>" );
fprintf( stderr, "%s\n", "<socket>          path to unix socket" );
fprintf( stderr, "\n%s\n\n", "OPTIONS" );
fprintf( stderr, "%s\n", "-h --help         help" );
fprintf( stderr, "%s\n", "-l --listen       listen (server mode)" );
fprintf( stderr, "%s\n", "-1 --once         exit program after connection close" );
fprintf( stderr, "%s\n", "-q --connqueue    maximum queue length of pending connections, default 1" );
fprintf( stderr, "%s\n", "-i                input file name, default stdin" );
fprintf( stderr, "%s\n", "-o                output file name, default stdout" );
fprintf( stderr, "%s\n", "-e --exec         system( <command> ) on every client connected" );
fprintf( stderr, "%s\n", "-c --predirect    redirect program to exec command" );
fprintf( stderr, "%s\n", "-p --credirect    redirect connection to exec command" );
fprintf( stderr, "%s\n", "-w --wait         wait <sec> seconds after EOF before closing a socket" );
fprintf( stderr, "%s\n", "-s --sync         use sync() after write to stdout" );
fprintf( stderr, "%s\n", "-n                enable stdin non-buffering mode" );
fprintf( stderr, "%s\n", "-b --buff         buffer for stdin, default 1K" );

examples();
}

