#ifndef IOH
#define IOH

void    io_init();
int     output_loop(int nready);
int     input_loop(char* sendbuff, int nready);

#endif
