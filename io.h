#ifndef IOH
#define IOH

void       io_init();
inline int output_loop(int nready);
inline int input_loop(char* sendbuff, int nready);

#endif
