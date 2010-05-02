#ifndef NETIOH
#define NETIOH

int main(int argc, char* argv[]);

char* cp(char* source);

void mylisten(char* addr, char* port);
void myconnect(char* addr, char* port);

void mywait(char* sec);

#endif
