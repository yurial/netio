#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "terminal.h" 

struct termios startT;

void GetStartTerminal()
{
ioctl( 0, TCGETS, &startT );
}

void SetStartTerminal()
{
ioctl( 0, TCSETS, &startT );
}

void nonbuffering()
{
struct termios T;
ioctl( 0, TCGETS, &T ); /* get current mode */
T.c_lflag &= ~ECHO;     /* echo off */
T.c_lflag &= ~ICANON;   /* one char @ a time */
ioctl( 0, TCSETS, &T ); /* set new terminal mode */
}

int setstdin(char* file)
{
int f_stdin = open( file, O_RDONLY );
if ( !f_stdin )
    return EXIT_FAILURE ;
dup2( f_stdin, fileno(stdin) );
close( f_stdin );
return EXIT_SUCCESS;
}

int setstdout(char* file)
{
int f_stdout = open( file, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR );
if ( !f_stdout )
    return EXIT_FAILURE;
dup2( f_stdout, fileno(stdout) );
close( f_stdout );
return EXIT_SUCCESS;
}

