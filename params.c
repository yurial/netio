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
char*   p_stdin        = NULL;
char*   p_stdout       = NULL;
char*   p_exec         = NULL;
char*   p_predirect    = NULL;
char*   p_credirect    = NULL;
int     p_wait         = 0;
int     p_sync         = 0;
int     p_nonbuffering = 0;
int     p_buffsize     = 1024;

const static char options[] = "hl1q:i:o:e:c:p:w:snb:";
const static struct option long_options[] = {
        { "help",      0, 0, 'h' },
        { "listen",    0, 0, 'l' },
        { "once",      0, 0, '1' },
	{ "connqueue", 1, 0, 'q' },
        { "exec",      1, 0, 'e' },
        { "predirect", 1, 0, 'p' },
        { "credirect", 1, 0, 'c' },
        { "wait",      1, 0, 'w' },
        { "sync",      0, 0, 's' },
        { "buff",      1, 0, 'b' },
        { 0,           0, 0,  0  }
    };

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
            fprintf( stderr, "long option!\n" );
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
        case 'i':
            {
            p_stdin = cp( optarg );
            }
            break;
        case 'o':
            {
            p_stdout = cp( optarg );
            }
            break;
        case 'e':
            {
            p_exec = cp( optarg );
            }
            break;
        case 'p':
            {
            p_predirect = cp( optarg );
            }
            break;
	case 'c':
	    {
	    p_credirect = cp( optarg );
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

/* collision check */
collision_check();

#ifdef DEBUG
int i;
for (i = 0; i < p_targetc; ++i)
    fprintf( stderr, "target: %s\n", p_targetv[i] );
fprintf( stderr, "server: %d\n", p_server );
fprintf( stderr, "once: %d\n", p_once );
fprintf( stderr, "connqueue: %d\n", p_connqueue );
fprintf( stderr, "stdin: %s\n", p_stdin );
fprintf( stderr, "stdout %s\n", p_stdout );
fprintf( stderr, "exec: %s\n", p_exec );
fprintf( stderr, "predirect: %s\n", p_predirect );
fprintf( stderr, "credirect: %s\n", p_credirect );
fprintf( stderr, "wait: %d\n", p_wait );
fprintf( stderr, "sync: %d\n", p_sync );
fprintf( stderr, "nonbuffering: %d\n", p_nonbuffering );
fprintf( stderr, "buffsize: %d\n", p_buffsize );
#endif
}
