#ifndef SERVERSH
#define SERVERSH

struct TServer
{
int             m_sock;
};

struct TServers
{
struct TServer* m_server;
int             m_count;
};

extern struct TServers g_servers;

int servers_loop(int nready);
void server_add(int socket);
void server_del(int socket);
void server_closeall();

#endif

