#ifndef PARAMSH
#define PARAMSH

extern char* p_target;
extern int   p_server;	//use as server
extern int   p_once;	//exit after socket closed
extern char* p_stdin;
extern char* p_stdout;
extern char* p_exec;
extern char* p_onconnect;
extern int   p_wait;
extern int   p_sync;
extern int   p_newline;
extern int   p_buffsize;

void params(int argc, char* argv[]);

#endif

