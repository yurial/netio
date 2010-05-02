#ifndef NETH
#define NETH

static const char STR_UNIX[] = "unix:";
static const char STR_TCP[] = "tcp://";
static const char STR_UDP[] = "udp://";
static const char STR_SCTP[] = "sctp://";
//static const char STR_UNIX[] = "";

enum PROTO
{
PROTO_UNKNOWN = 1,
PROTO_TCP,
PROTO_UDP,
PROTO_SCTP,
PROTO_UNIX
};

enum PROTO getproto(char* target);
size_t getprotolenght(enum PROTO proto);
char* gethost(enum PROTO proto, char* target);
int getport(enum PROTO proto, char* host, char* target);

#endif

