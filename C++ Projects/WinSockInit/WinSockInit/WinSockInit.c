#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void err_quit(char* msg)
{
	//"far" + pointer + void
	LPVOID lpMsgBuf;

	FormatMessage
	(
		//flag
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		//source
		NULL,
		//message id
		WSAGetLastError(),
		//language id
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		//Long Pointer t_string, buffer location
		(LPTSTR)&lpMsgBuf,
		//size
		0,
		//arguments
		NULL
	);

	MessageBox
	(
		//handle
		NULL,
		//text
		(LPCTSTR)lpMsgBuf,
		//caption
		msg,
		//type
		MB_ICONERROR
	);

	//pointer memory free Func
	LocalFree(lpMsgBuf);

	//error code
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	MessageBox(NULL, "WinSock 초기화 성공!", "알림", MB_OK);

	//Socket()

	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	MessageBox(NULL, "TCP 소켓 생성 성공!", "알림", MB_OK);

	//Closesocket()

	closesocket(tcp_sock);

	WSACleanup();

	return 0;
}