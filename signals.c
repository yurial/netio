#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "signals.h"
#include "clients.h"

void action_normterm();
void action_syncterm();
void action_fastterm();
void action_chld(pid_t pid);
void action_debug1();
void action_debug2();

void signal_HUP (int sig);
void signal_INT (int sig);
void signal_QUIT(int sig);
void signal_ILL (int sig);
void signal_ABRT(int sig);
void signal_FPE (int sig);
void signal_SEGV(int sig);
void signal_PIPE(int sig);
void signal_ALRM(int sig);
void signal_TERM(int sig);
void signal_USR1(int sig);
void signal_USR2(int sig);
void signal_CHLD(int sig);
void signal_CONT(int sig);
void signal_STOP(int sig);
void signal_TSTP(int sig);
void signal_TTIN(int sig);
void signal_TTOU(int sig);
void signal_PWR (int sig);

void signals_init()
{
signal( SIGHUP,  signal_HUP  );
signal( SIGINT,  signal_INT  );
signal( SIGQUIT, signal_QUIT );
signal( SIGILL,  signal_ILL  );
signal( SIGABRT, signal_ABRT );
signal( SIGFPE,  signal_FPE  );
signal( SIGSEGV, signal_SEGV );
signal( SIGPIPE, signal_PIPE );
signal( SIGALRM, signal_ALRM );
signal( SIGTERM, signal_TERM );
signal( SIGUSR1, signal_USR1 );
signal( SIGUSR2, signal_USR2 );
signal( SIGCHLD, signal_CHLD );
signal( SIGCONT, signal_CONT );
signal( SIGTSTP, signal_TSTP );
signal( SIGTTIN, signal_TTIN );
signal( SIGTTOU, signal_TTOU );
signal( SIGPWR,  signal_PWR  );
}

void signal_HUP(int sig)
{
action_syncterm();
}

void signal_INT(int sig)
{ 
action_normterm();
}

void signal_QUIT(int sig)
{
action_syncterm();
}

void signal_ILL(int sig)
{
action_fastterm();
}

void signal_ABRT(int sig)
{
action_fastterm();
}

void signal_FPE(int sig)
{
action_fastterm();
}

void signal_SEGV(int sig)
{
action_fastterm();
}

void signal_PIPE(int sig)
{
fprintf( stderr, "%s\n", "SIGPIPE" );
action_fastterm();
}

void signal_ALRM(int sig)
{
fprintf( stderr, "%s\n", "SIGALRM" );
}

void signal_TERM(int sig)
{
action_normterm();
}

void signal_USR1(int sig)
{
action_debug1();
}

void signal_USR2(int sig)
{
action_debug2();
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

void signal_CONT(int sig)
{
fprintf( stderr, "%s\n", "SIGCONT" );
}

void signal_TSTP(int sig)
{
fprintf( stderr, "%s\n", "SIGTSTP" );
}

void signal_TTIN(int sig)
{
fprintf( stderr, "%s\n", "SIGTTIN" );
}

void signal_TTOU(int sig)
{
fprintf( stderr, "%s\n", "SIGTTOU" );
}

void signal_PWR(int sig)
{
action_fastterm();
}

void action_normterm()
{
exit( EXIT_SUCCESS );
}

void action_syncterm()
{
exit( EXIT_SUCCESS );
}

void action_fastterm()
{
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
	shutdown( client->m_sock, SHUT_WR );
	client_tdisconnect( client );
	end = 1;
	}
    if ( client->m_opid == pid )
        {
	client->m_opid = -1;
	shutdown( client->m_sock, SHUT_RD );
	end = 1;
	}
    if ( end )
        {
	return;
	}
    ++client;
    }
}

void action_debug1()
{
#ifdef DEBUG
//TODO:
#endif
}

void action_debug2()
{
#ifdef DEBUG
//TODO:
#endif
}

