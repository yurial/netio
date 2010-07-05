#ifndef ERRORH
#define ERRORH

#include <errno.h>

void error_socket(int error);
void error_bind(int error);
void error_listen(int error);
void error_accept(int error);
void error_malloc(int error);

#endif
