#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <WinSock2.h>

typedef struct sockaddr {
	u_short sa_family;
	char sa_data[14];

}SOCKADDR;

//Socket Addr Struct = Address Field + Address(Little Endian)
typedef struct sockaddr_in {
	short sin_family;
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];

}SOCKADDR_IN;

typedef struct sockaddr_in6 {
	short sin6_family;
	u_short sin6_port;
	u_long sin6_flowinfo;
	struct in6_addr sin6_addr;
	u_long sin6_scope_id;

}SOCKADDR_IN6;

typedef struct in_addr {
	union
	{
		struct
		{
			u_char s_b1, s_b2, s_b3, s_b4;
		} S_un_b;
		struct
		{
			u_short s_w1, s_w2;
		}S_un_w;
		u_long S_addr;
	} S_un;
#define s_addr S_un.S_addr

}IN_ADDR;

typedef struct in6_addr
{
	union
	{
		u_char Byte[16];
		u_short Word[8];
	} u;
}IN6_ADDR;

