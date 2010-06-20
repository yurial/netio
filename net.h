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

#include "proto.h"

#ifdef USEIP4
#ifdef USETCP
	#define USETCP4
#endif
#endif

#ifdef USETCP
	static const char STR_NAME_TCP[]   = "tcp";
#endif
#ifdef USEUDP
	static const char STR_NAME_UDP[]   = "udp";
#endif
#ifdef USESCTP
	static const char STR_NAME_SCTP[]  = "stcp";
#endif
#ifdef USEUNIX
	static const char STR_NAME_UNIX[]  = "unix";
#endif

#ifdef USEUNIX
	static const char STR_UNIX[]  = "unix:";    
#endif
#ifdef USETCP4  
	static const char STR_TCP4[]  = "tcp://";   
#endif
#ifdef USEUDP4  
	static const char STR_UDP4[]  = "udp://";   
#endif
#ifdef USESCTP4 
	static const char STR_SCTP4[] = "sctp://";  
#endif
#ifdef USETCP6  
	static const char STR_TCP6[]  = "tcp6://";  
#endif
#ifdef USEUDP6  
	static const char STR_UDP6[]  = "udp6://";  
#endif
#ifdef USESCTP6 
	static const char STR_SCTP6[] = "sctp6://"; 
#endif
	//static const char STR_UNIX[] = "";

enum PROTO
{
#ifdef USEUNIX  
	PROTO_UNIX,  
#endif
#ifdef USETCP4 
	PROTO_TCP4,  
#endif
#ifdef USEUDP4 
	PROTO_UDP4,  
#endif
#ifdef USESCTP4 
	PROTO_SCTP4, 
#endif
#ifdef USETCP6  
	PROTO_TCP6,  
#endif
#ifdef USEUDP6  
	PROTO_UDP6,  
#endif
#ifdef USESCTP  
	PROTO_SCTP6, 
#endif
	PROTO_UNKNOWN
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
#ifdef USEUNIX  
	{ STR_UNIX,  sizeof(STR_UNIX) -1, STR_NAME_UNIX, PROTO_UNIX,  AF_UNIX,  SOCK_STREAM, IPPROTO_IP   }, 
#endif
#ifdef USETCP4  
	{ STR_TCP4,  sizeof(STR_TCP4) -1, STR_NAME_TCP,  PROTO_TCP4,  AF_INET,  SOCK_STREAM, IPPROTO_TCP  }, 
#endif
#ifdef USEUDP4  
	{ STR_UDP4,  sizeof(STR_UDP4) -1, STR_NAME_UDP,  PROTO_UDP4,  AF_INET,  SOCK_DGRAM,  IPPROTO_UDP  }, 
#endif
#ifdef USESCTP4 
	{ STR_SCTP4, sizeof(STR_SCTP4)-1, STR_NAME_SCTP, PROTO_SCTP4, AF_INET,  SOCK_STREAM, IPPROTO_SCTP }, 
#endif
#ifdef USETCP6  
	{ STR_TCP6,  sizeof(STR_TCP6) -1, STR_NAME_TCP,  PROTO_TCP6,  AF_INET6, SOCK_STREAM, IPPROTO_TCP  }, 
#endif
#ifdef USEUDP6  
	{ STR_UDP6,  sizeof(STR_UDP6) -1, STR_NAME_UDP,  PROTO_UDP6,  AF_INET6, SOCK_DGRAM,  IPPROTO_UDP  }, 
#endif
#ifdef USESCTP6 
	{ STR_SCTP6, sizeof(STR_SCTP6)-1, STR_NAME_SCTP, PROTO_SCTP6, AF_INET6, SOCK_STREAM, IPPROTO_SCTP }, 
#endif
        { NULL,      0,                   0,        0,           0,        0,           0 }
};

enum PROTO parsetarget(char* target, struct hostent** host, int* port);

#endif

