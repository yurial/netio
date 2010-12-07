#ifndef ERRORH
#define ERRORH

#include <errno.h>

void error_socket(int error);
void error_bind(int error);
void error_listen(int error);
void error_accept(int error);
void error_connect(int error);
void error_recv(int error);
void error_send(int error);
void error_read(int error);
void error_write(int error);
void error_malloc(int error);
void error_dup2(int error);
void error_close(int error);
void error_fork(int error);
void error_exec(int error);
void error_fsync(int error);
void error_poll(int error);
void error_pipe(int error);
void error_fcntl(int error);
void error_getitimer(int error);
void error_setitimer(int error);

#endif
