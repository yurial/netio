#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

#include "set.h"
#include "error.h"
#include "servers.h"
#include "clients.h"
#include "signals.h"
#include "params.h"
#include "run.h"

int g_ipid;
int g_opid;

void io_init()
{
int ret;
g_set = (struct pollfd*)malloc( sizeof(struct pollfd) * 2 );
g_set[STDOUT_FILENO].fd = STDOUT_FILENO;
g_set[STDOUT_FILENO].events = 0;
g_set[STDOUT_FILENO].revents = 0;
g_set[STDIN_FILENO].fd = STDIN_FILENO;
g_set[STDIN_FILENO].events = 0;
g_set[STDIN_FILENO].revents = 0;
g_setcount = 2;

if ( p_iomode == IOMODE_ONCE )
    {
    int newstdout[2];
    int newstdin[2];
    ret = pipe( newstdout );
    ret |= pipe( newstdin );
    if ( ret != 0 )
        {
        error_pipe( errno );
        exit( EXIT_FAILURE );
        }
    g_ipid = run( p_iocmd, newstdout[1], newstdin[0] );
    g_opid = g_ipid;
    if ( g_ipid == EXIT_FAILURE )
        {
        exit( EXIT_FAILURE );
        }
    int ret = dup2( newstdout[0], STDIN_FILENO );
    if ( ret == -1 )
        {
        error_dup2( errno );
        exit( EXIT_FAILURE );
        }
    ret = dup2( newstdin[1], STDOUT_FILENO );
    if ( ret == -1 )
        {
        error_dup2( errno );
        exit( EXIT_FAILURE );
        }
    }
else
    {
    if ( p_inmode == IOMODE_ONCE )
        {
        int newstdout[2];
        ret = pipe( newstdout );
        if ( ret != 0 )
            {
            error_pipe( errno );
            exit( EXIT_FAILURE );
            }
        g_ipid = run( p_incmd, newstdout[1], STDIN_FILENO );
        if ( g_ipid == EXIT_FAILURE )
            {
            exit( EXIT_FAILURE );
            }
        int ret = dup2( newstdout[0], STDIN_FILENO );
        if ( ret == -1 )
            {
            error_dup2( errno );
            exit( EXIT_FAILURE );
            }
        }
    if ( p_outmode == IOMODE_ONCE )
        {
        int newstdin[2];
        ret = pipe( newstdin );
        if ( ret != 0 )
            {
            error_pipe( errno );
            exit( EXIT_FAILURE );
            }
        g_opid = run( p_outcmd, STDOUT_FILENO, newstdin[0] );
        if ( g_opid == EXIT_FAILURE )
            exit( EXIT_FAILURE );
        int ret = dup2( newstdin[1], STDOUT_FILENO );
        if ( ret == -1 )
            {
            error_dup2( errno );
            exit( EXIT_FAILURE );
            }
        }
    }

int flags;
ret = fcntl( STDOUT_FILENO, F_GETFD, &flags );
if ( ret == -1 )
    {
    error_fcntl( errno );
    exit( EXIT_FAILURE );
    }
if ( p_sync )
    {
    flags |= O_SYNC;
    }
else
    {
    flags |= O_NONBLOCK;
    }
ret = fcntl( STDOUT_FILENO, F_SETFD, flags );
if ( ret == -1 )
    {
    error_fcntl( errno );
    exit( EXIT_FAILURE );
    }
}

int output_loop(int nready)
{
struct pollfd* set = g_set + STDOUT_FILENO;

if ( nready && !set->revents )
    return nready;

--nready;
if ( set->revents & POLLOUT )
    {
    set->revents ^= POLLOUT;
    set->events ^= POLLOUT;

    int clientindex = 0;
    int clientcount = g_clients.m_count;
    struct pollfd* set = g_set + client2set( 0 );
    while ( clientindex < clientcount );
        {
        set->events |= POLLIN;
        ++set;
        ++clientindex;
        }
     }
else
    {
    assert( 0 );
    }

return nready;
}

int input_loop(char* sendbuff, int nready)
{
struct pollfd* set = g_set + STDIN_FILENO;

if ( !(nready && set->revents) )
    return nready;

--nready;
if ( (set->revents & POLLHUP) && !(set->revents & POLLIN) )
    {
    set->revents ^= POLLHUP;
    set->events = 0;
    set->fd = -1;
    client_disconnectall();
    return nready;
    }

if ( set->revents & POLLIN )
    {
    set->revents ^= POLLIN;
    /* read from input */
    int readcount = read( STDIN_FILENO, sendbuff, p_sendbuff );
    if ( readcount == -1 )
        {
        error_read( errno );
        assert( readcount != -1 );
        return nready;
        }
    if ( readcount == 0 )
        {
        set->events = 0;
        set->fd = -1;
        client_disconnectall();
        return nready;
        }
    client_sendall( sendbuff, readcount );
    if ( g_clients.m_blocked > 0 )
        {
        set->events &= ~POLLIN;
        signals_cantsyncterm();
        }
    }
return nready;
}
