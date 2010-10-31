#ifndef SETH
#define SETH

#include <poll.h>

extern struct pollfd* g_set;
extern int g_setcount;

inline void set_add(int pos, int fd, int events);
inline void set_del(int pos);

#endif

