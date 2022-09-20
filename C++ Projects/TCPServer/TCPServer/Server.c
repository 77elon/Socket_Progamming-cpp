#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define SERVER_PORT 9000
#define BUFFER_SIZE 512

// 소켓 함수 오류 출력 후 종료
//general error message output Func
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

void err_display(char* msg)
{
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

	printf("[%s] %s", msg, (char*)lpMsgBuf);

	LocalFree(lpMsgBuf);

}

int main(int argc, char* argv[])
{
	int retval;

	//winsock init
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	
	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//serveraddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("listen()");
	}

	//데이터 통신 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;

	int addrlen;
	char buf[BUFFER_SIZE + 1];

	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}


		printf("\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		while (1)
		{
			retval = recv(client_sock, buf, BUFFER_SIZE, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			//받은 데이터 출력
			buf[retval] = 0;
			printf("[TCP/%s : %d] %s\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			//데이터 보내기
			retval = send(client_sock, buf, retval, 0);

			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}

		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소 = %s, 포트 번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	//closesocket()
	closesocket(listen_sock);

	//winsock 종료
	WSACleanup();

	return 0;
}
