#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include "params.h"
#include "help.h"
#include "cp.h"

#ifndef NULL
#define NULL 0
#endif

char* p_target = NULL;
int   p_server = 0;
int   p_once = 0;
char* p_stdin = NULL;
char* p_stdout = NULL;
char* p_exec = NULL;
char* p_onconnect = NULL;
int   p_wait = 0;
int   p_sync = 0;
int   p_newline = 0;
int   p_buffsize = 1024;

const static char options[] = "hl1i:o:e:c:w:snb:";
const static struct option long_options[] = {
        { "help",      0, 0, 'h' },
        { "listen",    0, 0, 'l' },
        { "once",      0, 0, '1' },
        { "exec",      1, 0, 'e' },
        { "onconnect", 1, 0, 'c' },
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
        case 'c':
            {
            p_onconnect = cp( optarg );
            }
            break;
        case 'w':
            {
fprintf( stderr, "test for numeric\n" );
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
            p_newline = 1;
            }
            break;
        case 'b':
            {
fprintf( stderr, "test for numeric\n" );
            p_buffsize = atoi( optarg );
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
    p_target = cp( argv[optind] );
else
    {
    fprintf( stderr, "<target> not specified!\n" );
    help();
    exit( EXIT_FAILURE );
    }

#ifdef DEBUG
fprintf( stderr, "target: %s\n", p_target );
fprintf( stderr, "server: %d\n", p_server );
fprintf( stderr, "once: %d\n", p_once );
fprintf( stderr, "stdin: %s\n", p_stdin );
fprintf( stderr, "stdout %s\n", p_stdout );
fprintf( stderr, "exec: %s\n", p_exec );
fprintf( stderr, "onconnect: %s\n", p_onconnect );
fprintf( stderr, "wait: %d\n", p_wait );
fprintf( stderr, "sync: %d\n", p_sync );
fprintf( stderr, "newline: %d\n", p_newline );
fprintf( stderr, "buffsize: %d\n", p_buffsize );
#endif
}
