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

void timer_setzero(struct timeval* value)
{
value->tv_sec = 0;
value->tv_usec = 0;
}

void timer_setnegative(struct timeval* value)
{
value->tv_sec = -1;
value->tv_usec = -1;
}

int timer_isequal(const struct timeval* lvalue, const struct timeval* rvalue)
{
return (lvalue->tv_sec == rvalue->tv_sec) && (lvalue->tv_usec == rvalue->tv_usec);
}

int timer_iszero(const struct timeval* value)
{
return (value->tv_sec == 0) && (value->tv_usec == 0);
}

int timer_isnegative(const struct timeval* value)
{
return (value->tv_sec < 0) || ((value->tv_sec == 0) && (value->tv_usec < 0));
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

    if ( timer_iszero( &minimal ) )
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
fprintf( stderr, "timer_init:\n  curr_value.it_value: %lu.%02lu\n  curr_value.it_interval: %lu.%02lu\n", curr_value.it_value.tv_sec, curr_value.it_value.tv_usec, curr_value.it_interval.tv_sec, curr_value.it_interval.tv_usec );
//TODO: ret
if ( timer_iszero( &curr_value.it_value ) )
    {
    timer_setzero( &curr_value.it_interval );
    curr_value.it_value = p_wait;
    ret = setitimer( ITIMER_REAL, &curr_value, NULL );
    //TODO: ret
    timer_setnegative( &client->m_timer );
fprintf( stderr, "\n  new_value.it_value: %lu.%02lu\n  new_value.it_interval: %lu.%02lu\n", curr_value.it_value.tv_sec, curr_value.it_value.tv_usec, curr_value.it_interval.tv_sec, curr_value.it_interval.tv_usec );
    return;
    }
else if ( timer_iszero( &curr_value.it_interval ) )
    {
    timer_sub( &curr_value.it_interval, &p_wait, &curr_value.it_value );
    ret = setitimer( ITIMER_REAL, &curr_value, NULL );
fprintf( stderr, "\n  new_value.it_value: %lu.%02lu\n  new_value.it_interval: %lu.%02lu\n\n", curr_value.it_value.tv_sec, curr_value.it_value.tv_usec, curr_value.it_interval.tv_sec, curr_value.it_interval.tv_usec );
    //TODO: ret
    }
timer_sub( &client->m_timer, &p_wait, &curr_value.it_value );
}

void timer_alarm()
{
/*disconnect*/
int index = 0;
while ( index < g_clients.m_count )
    {
    struct TClient* client = g_clients.m_client + index;
    if ( timer_isnegative( &client->m_timer ) )
        {
        client_disconnect( client );
        }
    else
        {
        ++index;
        }
    }
/**/
/* get minimal timeout */
struct timeval minimal = timer_getminimal();
/**/
/* set negative where minimal timeout else substract timeouts */
index = 0;
struct TClient* client = g_clients.m_client + index;
while ( index < g_clients.m_count )
    {
    if ( timer_isequal( &minimal, &client->m_timer) )
        {
        timer_setnegative( &client->m_timer );
        }
    else if ( !timer_iszero( &client->m_timer ) )
        {
        timer_sub( &client->m_timer, &client->m_timer, &minimal );
        }
    ++index;
    ++client;
    }
/**/
int ret;
struct itimerval curr_value;
ret = getitimer( ITIMER_REAL, &curr_value );
curr_value.it_interval = timer_getminimal();
ret = setitimer( ITIMER_REAL, &curr_value, NULL );
fprintf( stderr, "timer_alarm:\n  new_value.it_value: %lu.%02lu\n  new_value.it_interval: %lu.%02lu\n\n", curr_value.it_value.tv_sec, curr_value.it_value.tv_usec, curr_value.it_interval.tv_sec, curr_value.it_interval.tv_usec );
}

