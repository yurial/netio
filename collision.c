#include <stdlib.h>
#include <stdio.h>
#include "params.h"
#include "collision.h"

int collision_check()
{
if ( p_once && !p_server )
    {
    fprintf( stderr, "%s\n", "collision: --once can use only with --listen" );
    return  EXIT_FAILURE;
    }
if ( p_predirect && p_credirect )
    {
    fprintf( stderr, "%s\n", "collision: you can use or --predirect or --credirect not both" );
    return  EXIT_FAILURE;
    }
if ( (p_stdin || p_stdout) && (p_predirect || p_credirect) )
    {
    fprintf( stderr, "%s\n", "collision: you can use -o and -i without --predirect or --credirect" );
    return  EXIT_FAILURE;
    }
if ( p_wait && p_credirect )
    {
    fprintf( stderr, "%s\n", "collision: --wait with --credirect has no effect" );
    return  EXIT_FAILURE;
    }
return EXIT_SUCCESS;
}
