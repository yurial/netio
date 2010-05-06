#include <stdio.h>
#include "examples.h"

void examples()
{
fprintf( stderr, "\n%s\n", "EXAMPLE" );
fprintf( stderr, "\n%s\n", "$ netio microsoft.com:23" );
fprintf( stderr, "\n%s\n", "$ netio -l sctp://localhost:1000" );
fprintf( stderr, "\n%s\n", "$ netio -l unix:/tmp/mysocket" );
fprintf( stderr, "\n%s\n", "$ netio unix:../mysocket" );
fprintf( stderr, "\n%s\n", "$ echo \"MBX7Z86PRQYUHH13NKMTNQJGZY6ECTE=\" | netio -w 3 10.40.100.10:31337 >>answer" );
fprintf( stderr, "\n%s\n", "$ netio -snl -c\"netio -sn unix:/var/run/mysqld/mysqld.sock\" 0.0.0.0:3306" );
fprintf( stderr, "\n%s\n", "host1 $ netio -l1 -o dump.raw 0.0.0.0:1010" );
fprintf( stderr, "%s\n", "host2 # tcpdump -i wlan0 -w - |netio host1:1010" );
fprintf( stderr, "\n%s\n", "host1 # netio -sl -c/bin/bash 0.0.0.0:23" );
fprintf( stderr, "%s\n", "host2 $ netio -s host1:23" );
}

