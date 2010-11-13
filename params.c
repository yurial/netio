#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <signal.h>
#include "params.h"
#include "help.h"
#include "cp.h"
#include "collision.h"

char**          p_targetv      = NULL;
int             p_targetc      = 0;

int             p_server       = 0;
int             p_once         = 0;
int             p_connqueue    = 1;
char*           p_cmd          = "/bin/sh";
char*           p_incmd        = NULL;
enum iomode     p_inmode       = IOMODE_NONE;
char*           p_outcmd       = NULL;
enum iomode     p_outmode      = IOMODE_NONE;
char*           p_iocmd        = NULL;
enum iomode     p_iomode       = IOMODE_NONE;
int             p_chldterm     = SIGTERM; //TODO:
struct timeval  p_wait         = { 0 };
int             p_sync         = 0;
int             p_recvbuff     = 4096;
int             p_sendbuff     = 4096;

const static char options[] = "hl1q:c:i:o:w:sb:";
const static struct option long_options[] = {
	{ "io",        1, 0,  0  },
        { "rb",        1, 0,  0  },
	{ "sb",        1, 0,  0  },
        { "help",      0, 0, 'h' },
        { "listen",    0, 0, 'l' },
        { "once",      0, 0, '1' },
	{ "connqueue", 1, 0, 'q' },
	{ "cmd",       1, 0, 'c' },
        { "in",        1, 0, 'i' },
	{ "out",       1, 0, 'o' },
	{ "wait",      1, 0, 'w' },
        { "sync",      0, 0, 's' },
        { 0,           0, 0,  0  }
    };

char* getcmd(char* str, enum iomode* mode)
{
if ( !memcmp( str, "null", 4 ) )
    {
    *mode = IOMODE_NULL;
    return NULL;
    }
else if ( !memcmp( str, "block", 5) )
    {
    *mode = IOMODE_BLOCK;
    return NULL;
    }
else if ( !memcmp( str, "every:", 6 ) )
    {
    *mode = IOMODE_EVERY;
    return str + 6;
    }
else if ( !memcmp( str, "once:", 5 ) )
    {
    *mode = IOMODE_ONCE;
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
	    if ( option_index == 0 )
	        {
		p_iocmd = cp( getcmd( optarg, &p_iomode ) );
		}
            else if ( option_index == 1 )
                {
	        char* pend;
                p_recvbuff = strtol( optarg, &pend, 10 );
                if ( *pend == 'k' || *pend == 'K' )
	            p_recvbuff *= 1024;
	        else if ( *pend == 'm' || *pend == 'M' )
	            p_recvbuff *= 1024 * 1024;
	        }
            else if ( option_index == 2 )
                {
	        char* pend;
                p_sendbuff = strtol( optarg, &pend, 10 );
                if ( *pend == 'k' || *pend == 'K' )
	            p_sendbuff *= 1024;
	        else if ( *pend == 'm' || *pend == 'M' )
	            p_sendbuff *= 1024 * 1024;
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
            p_incmd = cp( getcmd( optarg, &p_inmode ) );
            }
            break;
        case 'o':
            {
            p_outcmd = cp( getcmd( optarg, &p_outmode ) );
            }
            break;
        case 'w':
            {
	    char* pend;
	    long msec = strtol( optarg, &pend, 10 );
            switch ( *pend )
	        {
		case 's':
	            msec *= 1000;
		    break;
	        case 'm':
	            msec *= 1000 * 60;
		    break;
		case 'h':
		    msec *= 1000 * 60 * 60;
		    break;
	        }
	    p_wait.tv_sec = msec / 1000;
	    p_wait.tv_usec = msec % 1000;
            }
            break;
        case 's':
            {
            p_sync = 1;
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
}
