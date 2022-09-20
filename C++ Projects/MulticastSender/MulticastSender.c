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

    // ��Ƽĳ��Ʈ TTL ����
    int ttl = 2;
    retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*) &ttl, sizeof(ttl));
    if(retval == SOCKET_ERROR) err_quit("socket()");

	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	remoteaddr.sin_port = htons(MULTICASTPORT);

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ��Ƽĳ��Ʈ ������ ������
    while(1)
    {
        // ������ �Է�
        printf("\n[���� ������] :");
        if(fgets(buf, BUFSIZE+1, stdin) == NULL)
        {
            break;
        }

        // '\n'  ���� ����
        len = strlen(buf);
        if(buf[len - 1] == '\n')
        {
            buf[len - 1] = '\0';
        }
        
        if(strlen(buf) == 0)
        {
            break;
        }


        //������ ������
        retval = sendto(sock, buf, strlen(buf), 0, (SOCKADDR *)&remoteaddr, sizeof(remoteaddr));

        if(retval == SOCKET_ERROR) {
            err_display("sendto()");
            continue;
        }
        printf("[UDP] %d ����Ʈ�� ���½��ϴ�. \n", retval);
    }

	// Closesocket()
	closesocket(sock);


	// ���� ����
	WSACleanup();
	return 0;
}