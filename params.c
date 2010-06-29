#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include "params.h"
#include "help.h"
#include "cp.h"
#include "collision.h"
#include "examples.h"

char**  p_targetv      = NULL;
int     p_targetc      = 0;

int     p_server       = 0;
int     p_once         = 0;
int     p_connqueue    = 1;
char*   p_cmd          = NULL;
char*   p_incmd        = NULL;
int     p_inevery      = 0;
char*   p_outcmd       = NULL;
int     p_outevery     = 0;
char*   p_iocmd        = NULL;
int     p_ioevery      = 0;
int     p_wait         = 0;
int     p_sync         = 0;
int     p_nonbuffering = 0;
int     p_buffsize     = 1024;

const static char options[] = "hl1q:c:i:o:w:snb:";
const static struct option long_options[] = {
	{ "io",        1, 0,  0  },
        { "help",      0, 0, 'h' },
        { "listen",    0, 0, 'l' },
        { "once",      0, 0, '1' },
	{ "connqueue", 1, 0, 'q' },
	{ "cmd",       1, 0, 'c' },
        { "in",        1, 0, 'i' },
	{ "out",       1, 0, 'o' },
	{ "wait",      1, 0, 'w' },
        { "sync",      0, 0, 's' },
        { "buff",      1, 0, 'b' },
        { 0,           0, 0,  0  }
    };

char* getcmd(char* str, int* every)
{
assert( every );

if ( !memcmp( str, "every:", 6 ) )
    {
    *every = 1;
    return str + 6;
    }
else if ( !memcmp( str, "once:", 5 ) )
    {
    *every = 0;
    return str + 5;
    }
return NULL;
}

void params(int argc, char* argv[])
{
int option;
int option_index;

while( 1 )
    {
    option = getopt_long(argc, argv, options, long_options, &option_index);
    if ( option == -1 )
        break;
    switch( option )
        {
	case 0:
            {
	    if (option_index == 0 )
	        {
		p_iocmd = cp( getcmd( optarg, &p_ioevery ) );
		}
	    }
            break;
        case 'h':
            {
            help();
            exit( EXIT_SUCCESS );
            }
            break;
        case 'l':
            {
            p_server = 1;
            }
            break;
        case '1':
            {
            p_once = 1;
            }
            break;
	case 'q':
	    {
	    p_connqueue = atoi( optarg );
	    }
	    break;
	case 'c':
	    {
            p_cmd = cp( optarg );
	    }
	    break;
        case 'i':
            {
            p_incmd = cp( getcmd( optarg, &p_inevery ) );
            }
            break;
        case 'o':
            {
            p_outcmd = cp( getcmd( optarg, &p_outevery ) );
            }
            break;
        case 'w':
            {
            p_wait = atoi( optarg );
            }
            break;
        case 's':
            {
            p_sync = 1;
            }
            break;
        case 'n':
            {
            p_nonbuffering = 1;
            }
            break;
        case 'b':
            {
	    char* pend;
            p_buffsize = strtol( optarg, &pend, 10 );
            if ( *pend == 'k' || *pend == 'K' )
	        p_buffsize *= 1024;
	    else if ( *pend == 'm' || *pend == 'M' )
	        p_buffsize *= 1024 * 1024;
	    }
            break;
        default:
            {
            help();
            exit( EXIT_FAILURE );
            }
            break;
        }
    }
if ( optind < argc )
    {
    p_targetc = argc - optind;
    p_targetv = malloc( sizeof(char*) * p_targetc );
    int ind;
    int argind;
    for (ind = 0, argind = optind; argind < argc; ++ind, ++argind)
        p_targetv[ind] = cp( argv[argind] );
    }
else
    {
    fprintf( stderr, "<target> not specified!\n" );
    help();
    exit( EXIT_FAILURE );
    }

#ifdef DEBUG
int i;
for (i = 0; i < p_targetc; ++i)
    fprintf( stderr, "target: %s\n", p_targetv[i] );
fprintf( stderr, "server: %d\n", p_server );
fprintf( stderr, "once: %d\n", p_once );
fprintf( stderr, "connqueue: %d\n", p_connqueue );
fprintf( stderr, "cmd: %s\n", p_cmd );
fprintf( stderr, "in: %s:%d\n", p_incmd, p_inevery );
fprintf( stderr, "out %s:%d\n", p_outcmd, p_outevery );
fprintf( stderr, "inout: %s:%d\n", p_iocmd, p_ioevery );
fprintf( stderr, "wait: %d\n", p_wait );
fprintf( stderr, "sync: %d\n", p_sync );
fprintf( stderr, "nonbuffering: %d\n", p_nonbuffering );
fprintf( stderr, "buffsize: %d\n", p_buffsize );
#endif

/* collision check */
collision_check();
}
