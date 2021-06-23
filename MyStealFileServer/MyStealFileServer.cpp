#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define MAX_SIZE 1024

//����̨��ӡ������ĺ���
void ErrorHanding(const char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	char msg[MAX_SIZE] = { 0 };

	wVersionRequested = MAKEWORD(2, 2);
	// 1����ʼ���׽��ֿ�
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		ErrorHanding("WSAStartup error");
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("LOBYTE errorNum = %d\n", GetLastError());
		WSACleanup();
		ErrorHanding("LOBYTE error");
		return -1;
	}
	// 2 ����socket
	SOCKET hServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == hServerSock)
	{
		ErrorHanding("socket error");
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	// 3 ����绰����
	 // ���׽��ֵ�����IP��ַ���˿ں�9527
	if (SOCKET_ERROR == bind(hServerSock, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
	{
		ErrorHanding("socket error");
	}

	// 4������ listen
	if (SOCKET_ERROR == listen(hServerSock, 5))
	{
		ErrorHanding("listen error");
	}

	SOCKADDR_IN addrCli;
	int cliAdrSize = sizeof(SOCKADDR_IN);
	SOCKET cliSock;
	int strLen = 0;
	// 5 ѭ����������
	while (TRUE)
	{
		cliSock = accept(hServerSock, (SOCKADDR*)&addrCli, &cliAdrSize);
		if (SOCKET_ERROR == cliSock)
		{
			ErrorHanding("accept error");
		}
		memset(msg, 0, MAX_SIZE);

		while ((strLen = recv(cliSock, msg, MAX_SIZE, 0)) != 0)
		{
			printf("Server msg = %s\n", msg);
		}
		closesocket(cliSock);
	}

	closesocket(hServerSock);
	WSACleanup();


	return 0;
}
