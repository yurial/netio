#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "set.h"
#include "servers.h"
#include "clients.h"
#include "params.h"

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

if ( nready && !set->revents )
    return nready;

--nready;
if ( set->revents & POLLIN )
    {
    set->revents ^= POLLIN;
    /* read from input */
    int readcount = read( STDIN_FILENO, sendbuff, p_buffsize );
    if ( readcount == -1 )
        {
        error_read( errno );
        assert( readcount != -1 );
        }
    int index;
    if ( readcount == 0 )
      {
        if ( p_wait == 0 )
            {
            int setindex = 0;
            struct pollfd* set = g_set;
            while ( setindex < g_setcount )
                {
                close( set->fd );
                ++set;
                ++setindex;
                }
            exit( EXIT_SUCCESS );
            }
        for (index = 0; index < g_clients.m_count; ++index)
            {
            clock_gettime( CLOCK_REALTIME, &g_clients.m_client[index].m_closetime );
            //TODO: run timer
            }
        }
    else
        {
        client_sendall( sendbuff, readcount );
        if ( g_clients.m_blocked > 0 )
            {
            set->events ^= POLLIN;
            }
        }
    }
else
    {
    assert( 0 );
    }

return nready;
}
