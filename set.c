#include <stdlib.h>
#include "set.h"

#define MAX(X,Y) (X>Y)?X:Y
#define MIN(X,Y) (X<Y)?X:Y

struct pollfd* g_set;
int g_setcount;

void set_add(int pos, int fd, int events)
{
int newsetcount = g_setcount + 1;
struct pollfd* newset = (struct pollfd*)malloc( sizeof(struct pollfd) * newsetcount );
int index;
for (index = 0; index < pos; ++index)
    {
    newset[index] = g_set[index];
    }

newset[index].fd = fd;
newset[index].events = events;

for (++index; index < newsetcount; ++index)
    {
    newset[index] = g_set[index-1];
    }
free( g_set );
g_set = newset;
g_setcount = newsetcount;
}

void set_del(int pos)
{
int newsetcount = g_setcount - 1;
struct pollfd* newset = (struct pollfd*)malloc( sizeof(struct pollfd) * newsetcount );
int index;
for (index = 0; index < pos; ++index)
    {
    newset[index] = g_set[index];
    }
for (; index < newsetcount; ++index)
    {
    newset[index] = g_set[index+1];
    }
free( g_set );
g_set = newset;
g_setcount = newsetcount;
}

