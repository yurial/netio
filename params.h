#ifndef PARAMSH
#define PARAMSH

extern char** p_targetv;
extern int    p_targetc;
extern int    p_server;
extern int    p_once;
extern int    p_connqueue;
extern char*  p_cmd;
extern char*  p_incmd;
extern int    p_inevery;
extern char*  p_outcmd;
extern int    p_outevery;
extern char*  p_iocmd;
extern int    p_ioevery;
extern int    p_wait;
extern int    p_sync;
extern int    p_nonbuffering;
extern int    p_buffsize;

void params(int argc, char* argv[]);

#endif

