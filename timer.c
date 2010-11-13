#include <sys/time.h>

#include "params.h"
#include "timer.h"

#include <stdio.h>

void timer_sub(struct timeval *result, const struct timeval *a, const struct timeval *b) /*r=a-b*/
{
int overflow = 0;
if ( a->tv_usec < b->tv_usec )
    {
    overflow = 1;
    result->tv_usec = b->tv_usec - a->tv_usec;
    }
else
    {
    result->tv_usec = a->tv_usec - b->tv_usec;
    }

result->tv_sec = a->tv_sec - b->tv_sec - overflow;
}

void timer_setzero(struct timeval *value)
{
value->tv_sec = 0;
value->tv_usec = 0;
}

void timer_setnegative(struct timeval *value)
{
value->tv_sec = -1;
value->tv_usec = -1;
}

int timer_iszero(const struct timeval *value)
{
return (value->tv_sec == 0) && (value->tv_usec == 0);
}

int timer_isnegative(const struct timeval *value)
{
return (value->tv_sec == -1) && (value->tv_usec == -1);
}

struct timeval timer_getminimal()
{
int index = 0;
struct TClient* client = g_clients.m_client;
struct timeval minimal;
timer_setzero( &minimal );
while ( index < g_clients.m_count )
    {
    if ( timer_iszero( &client->m_timer ) || timer_isnegative( &client->m_timer ) )
        {
	++client;
	++index;
	continue;
	}

    if ( timer_iszero( &client->m_timer ) )
        {
        minimal = client->m_timer;
	++client;
	++index;
	continue;
	}
    if ( client->m_timer.tv_sec < minimal.tv_sec )
        {
	minimal = client->m_timer;
	}
    else if ( client->m_timer.tv_sec == minimal.tv_sec )
        {
	if ( client->m_timer.tv_usec < minimal.tv_usec )
	    {
	    minimal = client->m_timer;
	    }
	}
    ++client;
    ++index;
    }
return minimal;
}

void timer_init(struct TClient* client)
{
int ret;
struct itimerval curr_value;
ret = getitimer( ITIMER_REAL, &curr_value );
//TODO: ret
if ( timer_iszero( &curr_value.it_value ) )
    {
    struct itimerval new_value;
    timer_setzero( &new_value.it_interval );
    new_value.it_value = p_wait;
    ret = setitimer( ITIMER_REAL, &new_value, NULL );
    //TODO: ret
    timer_setnegative( &client->m_timer );
    return;
    }
else if ( timer_iszero( &curr_value.it_interval ) )
    {
    timer_sub( &curr_value.it_interval, &p_wait, &curr_value.it_value );
    ret = setitimer( ITIMER_REAL, &curr_value, NULL );
    //TODO: ret
    }
timer_sub( &client->m_timer, &p_wait, &curr_value.it_value );
}

void timer_alarm()
{
/*disconnect*/
int index = 0;
struct TClient* client = g_clients.m_client;
while ( index < g_clients.m_count )
    {
    if ( timer_isnegative( &client->m_timer ) )
        {
        client_disconnect( client );
	}
    else
        {
	++client;
        ++index;
	}
    }
/**/
struct timeval minimal = timer_getminimal();
index = 0;
client = g_clients.m_client;
while ( index < g_clients.m_count )
    {
    if ( (client->m_timer.tv_sec == minimal.tv_sec) && (client->m_timer.tv_usec == minimal.tv_usec) )
        {
	timer_setnegative( &client->m_timer );
	}
    ++index;
    ++client;
    }
/**/
int ret;
struct itimerval curr_value;
ret = getitimer( ITIMER_REAL, &curr_value );
fprintf( stderr, "timer: %08x:%08x\n", (int)curr_value.it_value.tv_sec, (int)curr_value.it_value.tv_usec );
timer_setzero( &curr_value.it_interval );
ret = setitimer( ITIMER_REAL, &curr_value, NULL );
}

