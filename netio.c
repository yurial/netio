#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "netio.h"
#include "help.h"
#include "params.h"

int main(int argc, char* argv[])
{
if ( argc < 2 )
    {
    help();
    exit( EXIT_FAILURE );
    }

params( argc, argv );

if ( p_stdin )
    {
    int f_stdin = open( p_stdin, O_RDONLY );
    if ( !f_stdin )
        {
        fprintf( stderr, "Can't open input file '%s'\n", p_stdin );
        exit( EXIT_FAILURE );
        }
    dup2( f_stdin, fileno(stdin) );
    close( f_stdin );
    }

if ( p_stdout )
    {
    int f_stdout = open( p_stdout, O_WRONLY | O_CREAT | O_APPEND );
    if ( !f_stdout )
        {
        fprintf( stderr, "Can't open output file '%s'\n", p_stdout );
        exit( EXIT_FAILURE );
        }
    dup2( f_stdout, fileno(stdout) );
    close( f_stdout );
    }
    
if ( p_server )
    doserver();
else
    doclient();

return 0;
};
