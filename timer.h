#ifndef TIMERH
#define TIMERH

#include "clients.h"

int timer_iszero(const struct timeval *value);

void timer_init(struct TClient* client);
void timer_alarm();

#endif

