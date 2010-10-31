#ifndef NETIOH
#define NETIOH

extern struct TServers g_servers;
extern struct TClients g_clients;

int main(int argc, char* argv[]);
void sigquit(int signal);

#endif
