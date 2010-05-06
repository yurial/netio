#include <stdlib.h>
#include <unistd.h>
#include "run.h"

int run(char* command, int* in, int* out)
{
int pipe_in[2];
int pipe_out[2];

int ret;
ret = pipe( pipe_in );
if ( ret != 0 )
    return EXIT_FAILURE;
ret = pipe( pipe_out );
if ( ret != 0 )
    return EXIT_FAILURE;

pid_t pid = fork();
if ( pid == -1 )
    return EXIT_FAILURE;
if ( pid == 0 )
    {
    close( pipe_in[0] );
    close( pipe_out[1] );
    dup2( pipe_in[1], STDOUT_FILENO );
    dup2( pipe_in[1], STDERR_FILENO );
    dup2( pipe_out[0], STDIN_FILENO );
    execl( "/bin/sh", "sh", "-c", command, (char*)NULL );
    }
else
    {
    close( pipe_in[1] );
    close( pipe_out[0] );
    *in = pipe_in[0];
    *out = pipe_out[1];
    }
return pid;
}

