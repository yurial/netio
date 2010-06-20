#ifndef PARAMSH
#define PARAMSH

extern char** p_targetv;
extern int    p_targetc;
extern int    p_server;	//use as server
extern int    p_once;	//exit after socket closed
extern int    p_connqueue;
extern char*  p_cmd;
extern char*  p_inopt;
extern char*  p_outopt;
extern char*  p_ioopt;
extern int    p_wait;
extern int    p_sync;
extern int    p_nonbuffering;
extern int    p_buffsize;

void params(int argc, char* argv[]);

#endif

