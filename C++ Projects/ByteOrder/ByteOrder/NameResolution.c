#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <WinSock2.h>

#define TESTNAME "www.example.com"

//소켓 함수 오류 출력

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//Domain -> IPv4 Addr
BOOL GetIPAddr(char* name, IN_ADDR* addr)
{
	HOSTENT* ptr = gethostbyname(name);
	//if Request Address is empty
	if (ptr == NULL)
	{
		err_display("gethostbyname()");
		return FALSE;
	}
	//if not IPv4 Address 
	if (ptr->h_addrtype != AF_INET)
	{
		return FALSE;
	}
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return TRUE;
}

//IPv4 Address -> Domain (PTR)
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen)
{
	HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL)
	{
		err_display("gethostbyaddr()");
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET)
	{
		return FALSE;
	}
	strncpy(name, ptr->h_name, namelen);
	return TRUE;
}


int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	printf("도메인 이름 (변환 전) = %s\n", TESTNAME);

	//Domain -> IPv4 Addr
	IN_ADDR addr;

	//if Domain is available
	if (GetIPAddr(TESTNAME, &addr))
	{
		printf("IP 주소(변환 후) = %s\n", inet_ntoa(addr));

		//IPv4 -> Domain (PTR)
		char name[256];
		if (GetDomainName(addr, name, sizeof(name)))
		{
			printf("도메인 이름(다시 변환 후) = %s\n", name);
		}
	}
	WSACleanup();
	return 0;
}