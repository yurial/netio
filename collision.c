#include <stdlib.h>
#include <stdio.h>
#include "params.h"
#include "collision.h"

void collision_check()
{

if ( p_once && !p_server )
    {
    fprintf( stderr, "%s\n", "collision: --once can use only with --listen" );
    exit( EXIT_FAILURE );
    }
if ( p_iocmd && (p_incmd || p_outcmd) )
    {
    fprintf( stderr, "%s\n", "collision: you can use or --io or -i -o, not both" );
    exit( EXIT_FAILURE );
    }
}
