#ifndef SIGNALSH
#define SIGNALSH

void signals_init();

void signals_block();
void signals_unblock();

void signals_cansyncterm();
void signals_cantsyncterm();

int  signal_str(const char* str);

#endif
