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

struct net_param
{
const char* m_str;
size_t      m_strlength;
const char* m_protoname;
int         m_domain;
int         m_type;
int         m_protocol;
};

extern const struct net_param net_params[];

int parsetarget(char* target, struct hostent** host, int* port);

#endif

