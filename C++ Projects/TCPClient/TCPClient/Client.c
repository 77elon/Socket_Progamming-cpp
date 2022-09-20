#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define SERVERIP "192.168.7.1" //loop-back
#define SERVER_PORT 49152
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

//소켓 함수 오류 출력
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

//사용자 정의 데이터 수신 함수

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(
			//socket struct, object
			s,
			//buffer
			ptr,
			//left length
			left,
			//flags, default == 0
			flags);

		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		//Transfer end
		else if (received == 0)
		{
			break;
		}
		left -= received;
		ptr += received;
	}
	return (len - left);
}

int main(int argc, char *argv[])
{
	int retval;

	//winsock init
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	//socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	//connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	//데이터 통신에 사용할 변수
	char buf[BUFFER_SIZE + 1];
	int len;

	//서버와 데이터 통신
	while (1)
	{
		//데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFFER_SIZE + 1, stdin) == NULL)
		{
			break;
		}

		// '\n' 문자 제거 <- 사용자가 전달하는 텍스트에 개행문자가 포함되어 있으므로,,,!
		len = strlen(buf);
		if (buf[len - 1] == '\n')
		{
			buf[len - 1] = '\0';
		}
		
		if (strlen(buf) == 0)
		{
			break;
		}

		//데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		//데이터 받기
		retval = recvn(sock, buf, retval, 0);

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
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);

		printf("[받은 데이터] %s\n", buf);
	}


	//closesocket()
	closesocket(sock);

	//winsock 종료
	WSACleanup();

	return 0;
}