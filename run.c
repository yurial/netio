#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "run.h"
#include "params.h"
#include "error.h"

int run(char* command, int* in, int* out)
{
int pipe_in[2];
int pipe_out[2];

int ret;

if ( in )
    {
    ret = pipe( pipe_in );
    if ( ret != 0 )
        return EXIT_FAILURE;
    }

if ( out )
    {
    ret = pipe( pipe_out );
    if ( ret != 0 )
        return EXIT_FAILURE;
    }

pid_t pid = fork();
if ( pid == -1 )
    {
    error_fork( errno );
    return EXIT_FAILURE;
    }

if ( pid == 0 )
    {
    if ( in )
        {
        ret = close( pipe_in[0] );
        if ( ret != 0 )
	    {
	    error_close( errno );
	    assert( ret == 0 );
	    }
        ret = dup2( pipe_in[1], STDOUT_FILENO );
	if ( ret == -1 )
	    {
	    error_dup2( errno );
            return EXIT_FAILURE;
	    }
        }
    if ( out )
        {
	ret = close( pipe_out[1] );
        if ( ret != 0 )
	    {
	    error_close( errno );
	    assert( ret == 0 );
	    }
        ret = dup2( pipe_out[0], STDIN_FILENO );
	if ( ret == -1 )
	    {
	    error_dup2( errno );
	    return EXIT_FAILURE;
            }
	}
    execl( p_cmd, "sh", "-c", command, (char*)NULL );
    }
else
    {
    if ( in )
        {
	ret = close( pipe_in[1] );
        if ( ret != 0 )
	    {
	    error_close( errno );
	    assert( ret == 0 );
	    }
	*in = pipe_in[0];
        }
    if ( out )
        {
	ret = close( pipe_out[0] );
        if ( ret != 0 )
	    {
	    error_close( errno );
	    assert( ret == 0 );
	    }
	*out = pipe_out[1];
        }
    }
return pid;
}

