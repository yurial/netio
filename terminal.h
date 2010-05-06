#ifndef TERMINALH
#define TERMINALH

void GetStartTerminal();
void SetStartTerminal();

void nonbuffering();

int setstdin (char* file);
int setstdout(char* file);

#endif

