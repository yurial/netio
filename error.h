#ifndef ERRORH
#define ERRORH

#include <errno.h>

void error_socket(int error);
void error_malloc(int error);

#endif
