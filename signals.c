#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "signals.h"
#include "clients.h"
#include "timer.h"

#include "set.h"
#include "params.h"

sigset_t g_sigset;

void action_normterm();
void action_syncterm();
void action_fastterm();
void action_chld(pid_t pid);
void action_reexec();

void signal_handler(int sig);
void signal_ALRM(int sig);
void signal_CHLD(int sig);

struct str_signal
{
const char* m_str;
int         m_signal;
};

struct str_signal sigarr[] = {
    { "SIGHUP",  SIGHUP  },
    { "SIGINT",  SIGINT  },
    { "SIGQUIT", SIGQUIT },
    { "SIGILL",  SIGILL  },
    { "SIGABRT", SIGABRT },
    { "SIGFPE",  SIGFPE  },
    { "SIGKILL", SIGKILL },
    { "SIGSEGV", SIGSEGV },
    { "SIGPIPE", SIGPIPE },
    { "SIGALRM", SIGALRM },
    { "SIGTERM", SIGTERM },
    { "SIGUSR1", SIGUSR1 },
    { "SIGUSR2", SIGUSR2 },
    { "SIGCHLD", SIGCHLD },
    { "SIGCONT", SIGCONT },
    { "SIGSTOP", SIGSTOP },
    { "SIGTSTP", SIGTSTP },
    { "SIGTTIN", SIGTTIN },
    { "SIGTTOU", SIGTTOU }
};

int signal_str(const char* str)
{
int index = 0;
for (index = 0; index < sizeof(sigarr)/sizeof(struct str_signal); ++index)
    {
    if ( !strcmp( sigarr[index].m_str, str ) )
        return sigarr[index].m_signal;
    }
return atoi( str );
}

void signals_init()
{
sigemptyset( &g_sigset );
sigaddset( &g_sigset, SIGINT );
sigaddset( &g_sigset, SIGUSR1 );
sigaddset( &g_sigset, SIGUSR2 );
signals_cansyncterm();

signal( SIGHUP,  signal_handler );
signal( SIGINT,  signal_handler );
signal( SIGQUIT, signal_handler );
signal( SIGILL,  signal_handler );
signal( SIGFPE,  signal_handler );
signal( SIGSEGV, signal_handler );
signal( SIGTERM, signal_handler );
signal( SIGUSR1, signal_handler );
signal( SIGUSR2, signal_handler );
#ifdef SIGPWR
signal( SIGPWR,  signal_handler );
#endif
signal( SIGALRM, signal_ALRM );
signal( SIGCHLD, signal_CHLD );
//signal( SIGABRT, );
//signal( SIGPIPE, );
//signal( SIGCONT, );
//signal( SIGTSTP, );
//signal( SIGTTIN, );
//signal( SIGTTOU, );
}

void signal_handler(int sig)
{
switch ( sig )
{
case SIGINT:
    action_normterm();
    break;
case SIGHUP:
case SIGTERM:
case SIGQUIT:
    action_syncterm();
        break;
    case SIGILL:
    case SIGABRT:
    case SIGFPE:
    case SIGSEGV:
    case SIGPIPE:
#ifdef SIGPWR
   case SIGPWR:
        action_fastterm();
        break;
#endif
    case SIGUSR1:
        action_reexec();
        break;
    case SIGUSR2:
        debug_print();
        break;
    default:
        fprintf( stderr, "unknowh signal %08X\n", sig );
        break;
    };
}

void signals_block()
{
sigprocmask( SIG_BLOCK, &g_sigset, NULL );
}

void signals_unblock()
{
sigprocmask( SIG_UNBLOCK, &g_sigset, NULL );
}

void signals_cansyncterm()
{
sigaddset( &g_sigset, SIGHUP );
sigaddset( &g_sigset, SIGTERM );
sigaddset( &g_sigset, SIGQUIT );
}

void signals_cantsyncterm()
{
sigdelset( &g_sigset, SIGHUP );
sigdelset( &g_sigset, SIGTERM );
sigdelset( &g_sigset, SIGQUIT );
}

void signal_ALRM(int sig)
{
timer_alarm();
}

void signal_CHLD(int sig)
{
int status = 0;
pid_t pid = wait( &status );
if ( WNOHANG == status )
    {
    fprintf( stderr, "wait(): WNOHANG! WTF?\n" );
    }
else if ( WIFEXITED(status) )
    {
    action_chld( pid );
    }
//ignored:
//WUNTRACED(status)
//WCONTINUED(status)
//WEXITSTATUS(status)
//WIFSIGNALED(status)
//WTERMSIG(status)
//WCOREDUMP(status)
//WIFSTOPPED(status)
//WSTOPSIG(status)
//WIFCONTINUED(status)
}

void action_normterm()
{
int index;
for (index = 0; index < g_setcount; ++index)
    {
    close( g_set[index].fd );
    }
exit( EXIT_SUCCESS );
}

void action_syncterm()
{
int index;
for (index = 0; index < g_setcount; ++index)
    {
    close( g_set[index].fd );
    }
exit( EXIT_SUCCESS );
}

void action_fastterm()
{
int index;
for (index = 0; index < g_setcount; ++index)
    {
    close( g_set[index].fd );
    }
exit( EXIT_FAILURE );
}

void action_chld(pid_t pid)
{
int end = 0;
int index = 0;
struct TClient* client = g_clients.m_client;
while ( index < g_clients.m_count )
    {
    if ( client->m_ipid == pid )
        {
        client->m_ipid = -1;
        //TODO: shutdown( client->m_sock, SHUT_WR );
        client_tdisconnect( client );
        end = 1;
        }
    if ( client->m_opid == pid )
        {
        client->m_opid = -1;
        //TODO: shutdown( client->m_sock, SHUT_RD );
        end = 1;
        }
    if ( end )
        {
        return;
        }
    ++client;
    }
}

void action_reexec()
{
//TODO: reexec
fprintf( stderr, "reexec() %d\n", p_sync );
}

