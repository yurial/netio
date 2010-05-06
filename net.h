#ifndef NETH
#define NETH

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netdb.h>

static const char STR_TCP[]   = "tcp";
static const char STR_UDP[]   = "udp";
static const char STR_SCTP[]  = "stcp";
static const char STR_UNIX[]  = "unix:";
static const char STR_TCP4[]  = "tcp://";
static const char STR_UDP4[]  = "udp://";
static const char STR_SCTP4[] = "sctp://";
static const char STR_TCP6[]  = "tcp6://";
static const char STR_UDP6[]  = "udp6://";
static const char STR_SCTP6[] = "sctp6://";
//static const char STR_UNIX[] = "";

enum PROTO
{
PROTO_UNIX,
PROTO_TCP4,
PROTO_UDP4,
PROTO_SCTP4,
PROTO_TCP6,
PROTO_UDP6,
PROTO_SCTP6,
};

struct net_param
{
const char* m_str;
size_t      m_strlength;
const char* m_protoname;
enum PROTO  m_proto;
int         m_domain;
int         m_type;
int         m_protocol;
};

static const struct net_param net_params[] = {
    { STR_UNIX,  sizeof(STR_UNIX)-1,  NULL,     PROTO_UNIX,  AF_UNIX,  SOCK_STREAM, 0 },
    { STR_TCP4,  sizeof(STR_TCP4)-1,  STR_TCP,  PROTO_TCP4,  AF_INET,  SOCK_STREAM, 0 },
    { STR_UDP4,  sizeof(STR_UDP4)-1,  STR_UDP,  PROTO_UDP4,  AF_INET,  SOCK_DGRAM,  0 },
    { STR_SCTP4, sizeof(STR_SCTP4)-1, STR_SCTP, PROTO_SCTP4, AF_INET,  SOCK_STREAM, IPPROTO_SCTP },
    { STR_TCP6,  sizeof(STR_TCP6)-1,  STR_TCP,  PROTO_TCP6,  AF_INET6, SOCK_STREAM, 0 },
    { STR_UDP6,  sizeof(STR_UDP6)-1,  STR_UDP,  PROTO_UDP6,  AF_INET6, SOCK_DGRAM,  0 },
    { STR_SCTP6, sizeof(STR_SCTP6)-1, STR_SCTP, PROTO_SCTP6, AF_INET6, SOCK_STREAM, IPPROTO_SCTP },
    { NULL,      0,                   0,           0,        0,           0 }
};

//TODO:
void options_ipv4(char* options);
void options_ipv6(char* options);
void options_tcp (char* options);
void options_udp (char* options);
void options_sctp(char* options);

enum PROTO parsetarget(char* target, struct hostent** host, int* port);

#endif

