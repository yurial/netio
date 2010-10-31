#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "run.h"
#include "params.h"
#include "error.h"

pid_t run(const char* command, const int newstdout, const int newstdin)
{
int ret;

pid_t pid = fork();
if ( pid == -1 )
    {
    error_fork( errno );
    return -1;
    }

if ( pid == 0 )
    {
    ret = dup2( newstdout, STDOUT_FILENO );
    if ( ret == -1 )
        {
        error_dup2( errno );
        return -1;
        }
    ret = dup2( newstdin, STDIN_FILENO );
    if ( ret == -1 )
        {
        error_dup2( errno );
        return -1;
        }
    execl( p_cmd, p_cmd, "-c", command, (char*)NULL );
    error_exec( errno );
    exit( -1 );
    }

return pid;
}

