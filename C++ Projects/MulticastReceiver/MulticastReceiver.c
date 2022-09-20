#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#define MULTICASTIP "235.7.8.9"
#define MULTICASTPORT 9000
#define BUFSIZE 512

// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// Socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

    // SO_REUSEADDR ����
    BOOL optval = TRUE;
    retval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(optval));
    if(retval == SOCKET_ERROR) err_quit("setsockopt()_soreuseaddr");

	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	localaddr.sin_port = htons(MULTICASTPORT);
    retval = bind(sock, (SOCKADDR*)&localaddr, sizeof(localaddr));
    if(retval == SOCKET_ERROR) err_quit("bind()");

    // ��Ƽĳ��Ʈ �׷� ����
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICASTIP);
    mreq.imr_interface.s_addr = htonl("192.168.7.1");
    retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char* )&mreq, sizeof(mreq));
    if(retval == SOCKET_ERROR) err_quit("setsockopt()");

	// ������ ��ſ� ����� ����
    SOCKADDR_IN peeraddr;
	char buf[BUFSIZE + 1];
	int addrlen;

	// ��Ƽĳ��Ʈ ������ �ޱ�
    while(1)
    {
        // ������ �ޱ�
        addrlen = sizeof(peeraddr);
        retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR *)&peeraddr, &addrlen);

        if(retval == SOCKET_ERROR) {
            err_display("recvfrom()");
            continue;
        }

        // ���� ������ ���
        buf[retval] = '\0';
        printf("[UDP/%s:%d] %s \n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
    }


    // ��Ƽĳ��Ʈ �׷� Ż��
    retval =  setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    if(retval == SOCKET_ERROR) err_quit("setsockopt()");

	// Closesocket()
	closesocket(sock);


	// ���� ����
	WSACleanup();
	return 0;
}