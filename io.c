#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "set.h"
#include "error.h"
#include "servers.h"
#include "clients.h"
#include "params.h"

int g_ipid;
int g_opid;

void io_init()
{
int ret;
g_set = malloc( sizeof(struct pollfd) * 2 );
g_set[0].fd = STDOUT_FILENO;
g_set[0].events = 0;
g_set[0].revents = 0;
g_set[1].fd = STDIN_FILENO;
g_set[1].events = 0;
g_set[1].revents = 0;
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

inline int output_loop(int nready)
{
struct pollfd* set = g_set;

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

inline int input_loop(char* sendbuff, int nready)
{
struct pollfd* set = g_set + 1;

if ( !(nready && set->revents) )
    return nready;

--nready;
if ( set->revents & POLLHUP )
    {
    set->revents ^= POLLHUP;
    set->events = 0;
    set->fd = -1;
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
        set->fd = -1;
        set->events = 0;
        int index = 0;
        while ( index < g_clients.m_count )
            {
            struct TClient* client = g_clients.m_client + index;
            if ( client->m_timer.tv_sec != 0 || client->m_timer.tv_usec != 0 )
                {
                ++index;
                }
            else
                {
                client_tdisconnect( client );
                }
            }
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
