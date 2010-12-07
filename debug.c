#include <stdio.h>

#include "debug.h"

#include "params.h"
#include "servers.h"
#include "clients.h"
#include "error.h"

void debug_print_params()
{
int index;
fprintf( stderr, "params:\n" );
/*target*/
for (index = 0; index < p_targetc; ++index)
    {
    fprintf( stderr, "  target %04d: %s\n", index, p_targetv[index] );
    }
/*server*/
fprintf( stderr, "  p_server: %d\n", p_server );
/*once*/
fprintf( stderr, "  p_once: %d\n", p_once );
/*conqueue*/
fprintf( stderr, "  p_connqueue: %d\n", p_connqueue );
/*cmd*/
fprintf( stderr, "  p_cmd: %s\n", p_cmd );
/*incmd*/
fprintf( stderr, "  p_incmd: %s\n", p_incmd );
/*inmode*/
fprintf( stderr, "  p_inmode: %d\n", p_inmode );
/*outcmd*/
fprintf( stderr, "  p_outcmd: %s\n", p_outcmd );
/*outmode*/
fprintf( stderr, "  p_outmode: %d\n", p_outmode );
/*oicmd*/
fprintf( stderr, "  p_iocmd: %s\n", p_iocmd );
/*iomode*/
fprintf( stderr, "  p_iomode: %d\n", p_iomode );
/*chldterm*/
fprintf( stderr, "  p_chldterm: %d\n", p_chldterm );
/*wait*/
fprintf( stderr, "  p_wait: %ld.%ld\n", p_wait.tv_sec, p_wait.tv_usec );
/*sync*/
fprintf( stderr, "  p_sync: %d\n", p_sync );
/*recvbuff*/
fprintf( stderr, "  p_recvbuff: %d\n", p_recvbuff );
/*sendbuff*/
fprintf( stderr, "  p_sendbuff: %d\n", p_sendbuff );
}

void debug_print_servers()
{
fprintf( stderr, "%s\n", "servers:" );
int index = 0;
struct TServer* server = g_servers.m_server;
for (; index < g_servers.m_count; ++index, ++server)
    {
    fprintf( stderr, "  server %04d:\n", index );
    fprintf( stderr, "    socket: %d\n", server->m_sock );
    }
}

void debug_print_clients()
{
fprintf( stderr, "%s\n", "clients:" );
int index = 0;
struct TClient* client = g_clients.m_client;
for (; index < g_clients.m_count; ++index, ++client)
    {
    fprintf( stderr, "  client %04d:\n", index );
    fprintf( stderr, "    socket:   %d\n", client->m_sock );
    fprintf( stderr, "    sendbuff: %p\n", client->m_sendbuff );
    fprintf( stderr, "    remain:   %u\n", client->m_remain );
    fprintf( stderr, "    ipid:     %d\n", client->m_ipid );
    fprintf( stderr, "    opid:     %d\n", client->m_opid );
    fprintf( stderr, "    timer:    %0.3f\n", (double)client->m_timer.tv_sec + (double)client->m_timer.tv_usec / 1000000 );
    }
}

void debug_print_timer()
{
struct itimerval curr_value;
int ret = getitimer( ITIMER_REAL, &curr_value );
if ( ret == -1 )
    {
    error_getitimer( errno );
    //no need to exit
    }
fprintf( stderr, "timer:\n" );
fprintf( stderr, "  current value: %0.3f\n", (double)curr_value.it_value.tv_sec + (double)curr_value.it_value.tv_usec / 1000000 );
fprintf( stderr, "  next value:    %0.3f\n", (double)curr_value.it_interval.tv_sec + (double)curr_value.it_interval.tv_usec / 1000000 );
}

void debug_print()
{
debug_print_params();
debug_print_servers();
debug_print_clients();
debug_print_timer();
}

