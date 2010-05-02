#include <stdio.h>

void help()
{
fprintf( stderr, "%s\n\n", "NAME" );
fprintf( stderr, "%s\n", "netio - bind stdin & stdout with socket" );
fprintf( stderr, "\n%s\n\n", "SYNOPSIS" );
fprintf( stderr, "%s\n", "netio [-l [-1]] [-i <file>] [-o <file>] [-e <command>] [-c <command>] [-w <sec>] [-a] [-s] [-n] [-b <size[K|M]>] <target>" );
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
fprintf( stderr, "%s\n", "-i                input file name, if not specified use stdin" );
fprintf( stderr, "%s\n", "-o                output file name, if not specified use stdout" );
fprintf( stderr, "%s\n", "-e --exec         exec <command>, bind stdin and atdout with this program" );
fprintf( stderr, "%s\n", "-c --onconnect    exec <command> on client connected" );
fprintf( stderr, "%s\n", "-w --wait         wait <sec> seconds after EOF before closing a socket" );
fprintf( stderr, "%s\n", "-s --sync         use sync() after write to stdout" );
fprintf( stderr, "%s\n", "-n                send input data only when '\\n' exist or in buffer overflow" );
fprintf( stderr, "%s\n", "-b --buff         buffer for stdin, default 1K" );
fprintf( stderr, "\n%s\n", "EXAMPLE" );
fprintf( stderr, "\n%s\n", "$ netio microsoft.com:23" );
fprintf( stderr, "\n%s\n", "$ netio -l sctp://localhost:1000" );
fprintf( stderr, "\n%s\n", "$ netio -l unix:/tmp/mysocket" );
fprintf( stderr, "\n%s\n", "$ netio unix:../mysocket" );
fprintf( stderr, "\n%s\n", "$ echo \"MBX7Z86PRQYUHH13NKMTNQJGZY6ECTE=\" | netio -w 3 -n 10.40.100.10:30337 >>answer" );
fprintf( stderr, "\n%s\n", "$ netio -sl -e\"netio -s unix:/var/run/mysqld/mysqld.sock\" 0.0.0.0:3306" );
fprintf( stderr, "\n%s\n", "host1 $ netio -l1 -o dump.raw 0.0.0.0:1010" );
fprintf( stderr, "%s\n", "host2 # tcpdump -i wlan0 -w - |netio host1:1010" );
fprintf( stderr, "\n%s\n", "host1 # netio -s -l -e/bin/bash 0.0.0.0:23" );
fprintf( stderr, "%s\n", "host2 $ netio -s host1:23" );
//fprintf( stderr, "%s\n", "" );
}

