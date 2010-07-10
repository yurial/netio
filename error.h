#ifndef ERRORH
#define ERRORH

#include <errno.h>

void error_socket(int error);
void error_bind(int error);
void error_listen(int error);
void error_accept(int error);
void error_recv(int error);
void error_read(int error);
void error_malloc(int error);
void error_dup2(int error);
void error_close(int error);

#endif
