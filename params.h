#ifndef PARAMSH
#define PARAMSH

enum iomode
{
IOMODE_NONE,
IOMODE_NULL,
IOMODE_BLOCK,
IOMODE_ONCE,
IOMODE_EVERY,
};

extern char**         p_targetv;
extern int            p_targetc;
extern int            p_server;
extern int            p_once;
extern int            p_connqueue;
extern char*          p_cmd;
extern char*          p_incmd;
extern enum iomode    p_inmode;
extern char*          p_outcmd;
extern enum iomode    p_outmode;
extern char*          p_iocmd;
extern enum iomode    p_iomode;
extern int            p_chldterm;
extern int            p_wait;
extern int            p_sync;
extern int            p_sendbuff;
extern int            p_recvbuff;

void params(int argc, char* argv[]);

#endif

