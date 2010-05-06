#ifndef SETH
#define SETH

int* add(int* fds, int count, int element);
int* del(int* fds, int count, int index);
void mkfdset(fd_set* set);
int  mknfds();

#endif

