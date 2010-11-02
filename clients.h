#ifndef CLIENTSH
#define CLIENTSH

#include <poll.h>
#include <time.h>
#include <string.h>

struct TClient
{
int             m_sock;
char*           m_sendbuff;
size_t          m_remain;
int             m_ipid;
int             m_opid;
struct timespec m_closetime;
};

struct TClients
{
struct TClient* m_client;
int             m_count;
int             m_blocked;
};

extern struct TClients g_clients;

void client_add(int socket);

int client_POLLOUT(int nready, struct pollfd* set, struct TClient* client, char* recvbuff);
int client_POLLIN (int nready, struct pollfd* set, struct TClient* client, char* recvbuff);

void client_sendall(char* buff, const size_t buffsize);

void client_disconnect(struct TClient* client);
void client_tdisconnect(struct TClient* client);

#endif

