#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <math.h>
#include <time.h>
#include <malloc.h>

#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include <windows.h>



#pragma comment( lib, "ws2_32.lib")

#define BUFSIZE 1024

int main()
{

	WSADATA wsa_data;
	SOCKET clnt_sock;
	SOCKADDR_IN serv_addr;



	char sAddr[15] = {0};
	int sPort;

	char data_send[BUFSIZE] = { 0 };

	printf("서버 IP : ");
	//gets(sAddr);
	scanf_s("%s", sAddr,15);
	printf("포트 번호 : ");
	scanf_s("%s", data_send,BUFSIZE);
	//gets(message);
	sPort = atoi(data_send);


	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Load WinSock 2.2 DLL Error");
		return 1;
	}
	
	clnt_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clnt_sock == INVALID_SOCKET)
	{
		printf("Socket Error");
		WSACleanup();
		return 1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(sPort);
	serv_addr.sin_addr.s_addr = inet_addr(sAddr);
	//inet_pton(AF_INET, sAddr, &serv_addr.sin_addr);

	if (connect(clnt_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		printf("Connection Error\n");
		closesocket(clnt_sock);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("연결 성공,,,\n시작,,,\n");
	}

	int nRcv;
	char data_recv[BUFSIZE] = { 0 };


	while (1)
	{
		printf("\n보낼 메세지 : ");
		scanf_s("%s", data_send,BUFSIZE);
		//gets(message);

		if (strcmp(data_send, "exit") == 0)
		{
			send(clnt_sock, data_send, strlen(data_send), 0);
			break;
		}

		send(clnt_sock, data_send, strlen(data_send), 0);
		
		printf("메세지 받는중,,,\n");

		nRcv = recv(clnt_sock, data_recv, sizeof(data_recv), 0);
		if (nRcv == SOCKET_ERROR)
		{
			printf("Receive Error\n");
			break;
		}


		if (strcmp(data_recv, "exit") == 0)
		{
			printf("상대방이 대화를 종료 했습니다,,,\n");
			break;
		}

		printf("받은 메세지 : %s", data_recv);
	}

	closesocket(clnt_sock);
	WSACleanup();

	printf("열결 끊는중,,,\n");

	//_getch();

	return 0;
}
