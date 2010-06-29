#ifndef SETH
#define SETH

void sigquit(int signal);

struct TArray
{
int* m_array;
int  m_count;
};

void add(struct TArray* array, int element);
void del(struct TArray* array, int index);
void mkfdset(fd_set* set);
int  mknfds();

#endif

