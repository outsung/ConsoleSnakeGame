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

//#define BUFSIZE 1024


int main()
{
	WSADATA wsa_data;
	SOCKET listen_sock;
	SOCKADDR_IN serv_addr;



	//char message[BUFSIZE];

	//int str_len;
	int nRcv;

	int sPort = 30002;

	printf("열 포트 번호 : ");
	scanf_s("%d", &sPort);
	//gets(sPort);



	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Load WinSock 2.2 DLL Error\n");
		return 1;
	}
		

	listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Socket Error\n");
		return 1;
	}


	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(sPort);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listen_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		printf("Bind Error\n");
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}
	if (listen(listen_sock, 5) == SOCKET_ERROR)
	{
		printf("Listen Error\n");
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}
	
	
	SOCKADDR clnt_addr;
	SOCKET clnt_sock;

	int size_clnt_addr = sizeof(clnt_addr);

	char date_send[1024] = { 0 };
	char recv_byte;
	char date_recv[1024] = { 0 };



	clnt_sock = accept(listen_sock, (SOCKADDR*)& clnt_addr, &size_clnt_addr);
	if (clnt_sock == INVALID_SOCKET)
	{
		printf("Accept Error\n");
		closesocket(listen_sock);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("%d : 연결 성공,,,\n",clnt_sock);
		printf("시작,,,\n");
	}

	closesocket(listen_sock);

	while (1)
	{
		printf("메세지 받는중,,,\n");
		nRcv = recv(clnt_sock, date_recv, sizeof(date_recv), 0);

		if (nRcv == SOCKET_ERROR)
		{
			printf("Receive Error\n");
			break;
		}
		if (strcmp(date_recv, "exit") == 0)
		{
			printf("상대방이 대화를 종료 했습니다,,,\n");
			break;
		}

		printf("받은 메세지 : %s", date_recv);
		printf("\n보낼 메세지 : ");

		scanf_s("%s", date_send,1024);

		//gets_s(message, sizeof(message));
		//gets(message);

		if (strcmp(date_send, "exit") == 0)
		{
			send(clnt_sock, date_send, strlen(date_send), 0);
			break;
		}

		send(clnt_sock, date_send, strlen(date_send), 0);
	}

	closesocket(clnt_sock);
	WSACleanup();

	printf("열결 끊는중,,,\n");

	//_getch();

	return 0;
}
