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


// 필수 메크로 정의
#define MAX_PLAYER 4


// 필수 변수 정의
typedef struct MouseInfo
{
	COORD MousePostion;
	int PressRight;
	int PressLeft;

}MouseInfo;

typedef struct PlayerPositin
{
	int x;
	int y;

}PlayerPositin;

MouseInfo UserMouse = { 0 };

int StartMenu;


// 필수 함수 정의
int IsKeyDown(int key)
{
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

void CSgotoxy(int x, int y)
{
	COORD CursorPosition = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

void T_MouseInput()
{
	INPUT_RECORD rec;
	DWORD dw, dwmode;
	HANDLE hIn;

	clock_t Attacked_time = 0, time = 0;

	hIn = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hIn, GetConsoleMode(hIn, &dwmode) | ENABLE_MOUSE_INPUT);

	for (;;)
	{
		time = clock();

		ReadConsoleInput(hIn, &rec, 1, &dw); // 사용자로부터 입력을 받습니다.

		if (rec.EventType == MOUSE_EVENT) // 사용자로부터 받은 입력이 마우스일 경우,
		{
			UserMouse.MousePostion.X = rec.Event.MouseEvent.dwMousePosition.X;
			UserMouse.MousePostion.Y = rec.Event.MouseEvent.dwMousePosition.Y;

			//printf("A");// 커서의 위치를 왼쪽 버튼이 눌려진 위치로 이동합니다.

			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) // 왼쪽 버튼이 눌렸을 경우,
				UserMouse.PressLeft = 1;
			else
				UserMouse.PressLeft = 0;

			if (rec.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) // 오른쪽 버튼이 눌렸을 경우,
				UserMouse.PressRight = 1;
			else
				UserMouse.PressRight = 0;

		}
	}
}

void SetColor(unsigned short color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void G_Clean()
{
	COORD sCoord = { 0,0 };
	DWORD dw;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 300 * 100, sCoord, &dw);
}

// Clnt
void T_N_cSend(void* _pArgs)
{	
	/*
		Send 내용
		{
			Send_ID

			player position
			mouseInfo
		}
	*/

	SOCKET cSocket = *(SOCKET*)_pArgs;

	char Data_Send[100] = { 0 };
	int Data_SendSize;

	while (TRUE)
	{
		/*sprintf(Data_Send, "", );
			Data_Send에 대입
		*/
		Data_SendSize = strlen(Data_Send);
		send(cSocket, Data_Send, Data_SendSize, 0);
	}
	

	//sprintf(system_str, "mode con cols=%d lines=%d", nWidth / FsizeX, nHeight / FsizeY);
	//send(cSocket, data_send, strlen(data_send), 0);

}
void T_N_cRecv(void* _pArgs)
{
	/*
	Secv 내용
	{
		Send_ID 
		
		0
		player position
		mouseInfo
		1
		player position
		mouseInfo
		2
		player position
		mouseInfo
		3
		player position
		mouseInfo
	}
	*/

	SOCKET cSocket = *(SOCKET*)_pArgs;

	char Data_Recv[1024] = { 0 };
	int Data_RecvSize;

	do
	{
		Data_RecvSize = recv(cSocket, Data_Recv, sizeof(Data_Recv), 0);
		
		/*
			Data_Recv 처리

		*/

	} while (Data_RecvSize > 0);

}

// Serv
void T_N_sSend()
{
	/*
	Send 내용
	{
		0~3
		Send_ID 

		0
		player position
		mouseInfo
		1
		player position
		mouseInfo
		2
		player position
		mouseInfo
		3
		player position
		mouseInfo
	} * 4
	*/

}
void T_N_sRecv()
{
	/*
	Send 내용
	{
		player position
		mouseInfo
	} * 4
	*/

}


// 함수 정의
void B_ConsloeInit();
int P_Start();
void G_Title(int x, int y);
//void G_Menu();
void G_Box(int x, int y, int w, int h);
int N_ServerMake_Main();
void N_ServerConnection();
void P_GameExit();
void P_Game_Start();


int main()
{
	// 기본 콘솔 정리
	B_ConsloeInit();

	// 마우스 입력 받기
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)T_MouseInput, NULL, 0, NULL);

	// 시작 화면
	StartMenu = P_Start();

	switch (StartMenu)
	{
	case 1:
		// -- 1. 서버 만들기 --
		N_ServerMake_Main();
		break;

	case 2:
		// -- 2. 서버 들어가기 --
		N_ServerConnection();
		P_Game_Start();
		break;

	case 3:
		// -- 3. 게임 나가기 --
		P_GameExit();
		break;

	default:
		break;
	}

	return 0;
}

// 콘솔 초기화
void B_ConsloeInit()
{
	CONSOLE_CURSOR_INFO cr;

	cr.dwSize = 1;
	cr.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cr);

	int ScreenWidth, ScreenHeight;
	int FntSizeX, FntSizeY;

	int ConsoleWidth, ConsoleHeight;

	char system_str[35] = { 0 };

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	CONSOLE_FONT_INFO Fn_;
	GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &Fn_);

	FntSizeX = Fn_.dwFontSize.X;
	FntSizeY = Fn_.dwFontSize.Y;

	ConsoleWidth = ScreenWidth / FntSizeX;
	ConsoleHeight = ScreenHeight / FntSizeY - 4;

	sprintf(system_str, "mode con cols=%d lines=%d", ConsoleWidth, ConsoleHeight);
	system(system_str);

	system("title Out Zombie");
	

}

// 시작화면
int P_Start()
{
	//G_Title(15, 5);
	while (TRUE)
	{
		// 서버 만들기 박스
		if ((UserMouse.MousePostion.X >= 130 && UserMouse.MousePostion.X <= 130 + 10 * 2) &&
			(UserMouse.MousePostion.Y >= 9 && UserMouse.MousePostion.Y <= 9 + 5))
		{
			if (UserMouse.PressLeft == 1)
				return 1;
			SetColor(14);
		}
		else SetColor(15);

		G_Box(130, 9, 10, 5);
		CSgotoxy(134, 11);
		printf("서버  만들기");


		// 서버 들어가기 박스
		if ((UserMouse.MousePostion.X >= 152 && UserMouse.MousePostion.X <= 152 + 10 * 2) &&
			(UserMouse.MousePostion.Y >= 9 && UserMouse.MousePostion.Y <= 9 + 5))
		{
			if (UserMouse.PressLeft == 1)
				return 2;
			SetColor(14);
		}
		else SetColor(15);

		G_Box(152, 9, 10, 5);
		CSgotoxy(155, 11);
		printf("서버  들어가기");

		// 게임 나가기 박스
		if ((UserMouse.MousePostion.X >= 174 && UserMouse.MousePostion.X <= 174 + 10 * 2) &&
			(UserMouse.MousePostion.Y >= 9 && UserMouse.MousePostion.Y <= 9 + 5))
		{
			if (UserMouse.PressLeft == 1)
				return 3;
			SetColor(14);
		}
		else SetColor(15);

		G_Box(174, 9, 10, 5);
		CSgotoxy(178, 11);
		printf("게임  나가기");
	}

}
// 시작화면 그리기
void G_Title(int x, int y)
{
	CSgotoxy(x, y++);
	printf("         ________                           _______                 ______");
	CSgotoxy(x, y++);
	printf("        /:::＼   ＼                        /:＼    ＼              /:＼   ＼");
	CSgotoxy(x, y++);
	printf("       /::::::＼   ＼                     /::::＼____＼           /::::＼   ＼");
	CSgotoxy(x, y++);
	printf("      /:::::::::＼   ＼                  /::::/      /            ＼:::::＼   ＼");
	CSgotoxy(x, y++);
	printf("     /::::::::::::＼   ＼               /::::/      /               ＼:::::＼   ＼");
	CSgotoxy(x, y++);
	printf("    /::::/ ~~＼:::::＼   ＼            /::::/      /                  ＼:::::＼   ＼");
	CSgotoxy(x, y++);
	printf("   /::::/      ＼:::::＼   ＼         /::::/      /                     ＼:::::＼   ＼");
	CSgotoxy(x, y++);
	printf("  /::::/       / ＼:::::＼   ＼      /::::/      /                       /:::::::＼   ＼");
	CSgotoxy(x, y++);
	printf(" /::::/_______/    ＼:::::＼___＼   /::::/      /        _____          /::::::::::＼   ＼");
	CSgotoxy(x, y++);
	printf("|::::|       |      |::::::|    |  /::::/______/        /:＼  ＼       /:::::/ ＼::::＼   ＼");
	CSgotoxy(x, y++);
	printf("|::::|_______|      |::::::|    | |::::|      /        /::::＼__＼    /:::::/  / ＼::::＼___＼");
	CSgotoxy(x, y++);
	printf(" ＼:::＼      ＼   /::::::/    /  |::::|______＼      /::::::/   /   /:::::/  /    ＼:::/    /");
	CSgotoxy(x, y++);
	printf("   ＼:::＼      ＼/::::::/    /    ＼:::＼      ＼   /::::::/   /   /:::::/  /       ＼/____/");
	CSgotoxy(x, y++);
	printf("     ＼:::＼     /::::::/    /       ＼:::＼      ＼/::::::/   /   /:::::/  /");
	CSgotoxy(x, y++);
	printf("       ＼:::＼__/ :::::/    /          ＼:::＼     /::::::/   /   /:::::/  /");
	CSgotoxy(x, y++);
	printf("         ＼:::::::::::/    /             ＼:::＼__/::::::/   /    ＼:::/  /");
	CSgotoxy(x, y++);
	printf("           ＼::::::::/    /                ＼:::::::::::/   /       ＼/__/");
	CSgotoxy(x, y++);
	printf("             ＼:::::/    /                   ＼::::::::/   /");
	CSgotoxy(x, y++);
	printf("               ＼::/____/                      ＼:::::/   /");
	CSgotoxy(x, y++);
	printf("                 ~~                              ＼::/___/");
	CSgotoxy(x, y++);
	printf("                                                   ~~");

	y++;
	y++;
	y++;
	y++;
	y++;
	y++;
	y++;

	CSgotoxy(x, y++);
	printf("   _____                              ________                            ______                          ______                                _____                                 _____");
	CSgotoxy(x, y++);
	printf("  /＼   ＼                           /:::＼   ＼                         /:＼   ＼                       /＼    ＼                            /＼   ＼                              /＼   ＼");
	CSgotoxy(x, y++);
	printf(" /:::＼   ＼                        /::::::＼   ＼                      /::::＼___＼                    /:::＼    ＼                         /:::＼   ＼                           /:::＼   ＼");
	CSgotoxy(x, y++);
	printf(" ＼::::＼   ＼                     /:::::::::＼   ＼                   /::::: |    |                   /::::::＼    ＼                       ＼::::＼   ＼                        /::::::＼   ＼");
	CSgotoxy(x, y++);
	printf("   ＼::::＼   ＼                  /::::::::::::＼   ＼                /:::::::|    |                  /:::::::::＼    ＼                       ＼::::＼   ＼                     /:::::::::＼   ＼");
	CSgotoxy(x, y++);
	printf("     ＼::::＼   ＼               /::::/~~＼::::::＼   ＼             /::::::::|    |                 /:::/＼::::::＼    ＼                       ＼::::＼   ＼                  /:::: /＼::::＼   ＼");
	CSgotoxy(x, y++);
	printf("       ＼::::＼   ＼            /::::/     ＼::::::＼   ＼          /:::::/|::|    |                /:::/ __＼::::::＼    ＼                       ＼::::＼   ＼               /:::::/__ ＼::::＼   ＼");
	CSgotoxy(x, y++);
	printf("         ＼::::＼   ＼         /::::/      / ＼::::::＼   ＼       /:::::/ |::|    |               /::::＼    ＼::::::＼    ＼                      /::::::＼   ＼            /::::::＼    ＼::::＼   ＼");
	CSgotoxy(x, y++);
	printf("           ＼::::＼   ＼      /::::/______/    ＼::::::＼___＼    /:::::/  |::|____| ____         /:::::::＼    ＼::::::＼    ＼      ____         /:::::::::＼   ＼         /:::::::::＼    ＼::::＼   ＼");
	CSgotoxy(x, y++);
	printf("             ＼::::＼   ＼   |::::|      |       |::::::|    |   /:::::/   |:::::::::＼  ＼      /::::/＼:::＼    ＼::::::＼ ___＼   /＼  ＼      /:::::/＼::::＼   ＼      /:::::/＼::::＼    ＼::::＼   ＼");
	CSgotoxy(x, y++);
	printf("_______________＼::::＼___＼ |::::|______|       |::::::|    |  /:::::/    |:::::::::::＼__＼   /::::/__ ＼:::＼    ＼::::::|    |  /:::＼  ＼   /:::::/   ＼::::＼___＼   /:::::/ __＼::::＼    ＼::::＼___＼");
	CSgotoxy(x, y++);
	printf("＼:::::::::::::::::::/     /  ＼:::＼     ＼    /::::::/    /   ＼:::/    / ~~~~~/:::::/    /   ＼:::＼    ＼:::＼   /::::::|____|  ＼::::＼  ＼/:::::/    / ＼:::/    /   ＼::::＼    ＼::::＼    ＼:::/    /");
	CSgotoxy(x, y++);
	printf("  ＼::::::::::::::::/_____/     ＼:::＼     ＼ /::::::/    /      ＼/____/      /:::::/    /      ＼:::＼    ＼:::＼/::::::/    /     ＼::::＼ /:::::/    /    ＼/____/      ＼::::＼    ＼::::＼    ＼/___ /");
	CSgotoxy(x, y++);
	printf("    ＼::::＼~~~~＼~~~~~~          ＼:::＼     /::::::/    /                    /:::::/    /         ＼:::＼    ＼:::::::::/    /        ＼::::::::::/    /                     ＼::::＼    ＼::::＼    ＼");
	CSgotoxy(x, y++);
	printf("      ＼::::＼    ＼                ＼:::＼__/::::::/    /                    /:::::/    /            ＼:::＼    ＼::::::/    /           ＼:::::::/____/                        ＼::::＼    ＼::::＼___ ＼");
	CSgotoxy(x, y++);
	printf("        ＼::::＼    ＼                ＼:::::::::::/    /                    /:::::/    /               ＼:::＼   /:::::/    /              ＼:::::＼   ＼                         ＼::::＼    ＼:::/     /");
	CSgotoxy(x, y++);
	printf("          ＼::::＼    ＼                ＼::::::::/    /                    /:::::/    /                  ＼:::＼/:::::/    /                 ＼:::::＼   ＼                         ＼::::＼    ＼/____ /");
	CSgotoxy(x, y++);
	printf("            ＼::::＼    ＼                ＼:::::/    /                    /:::::/    /                     ＼::::::::/    /                    ＼:::::＼   ＼                         ＼::::＼    ＼");
	CSgotoxy(x, y++);
	printf("              ＼::::＼____＼                ＼::/____/                    /:::::/    /                        ＼:::::/    /                       ＼:::::＼___＼                         ＼::::＼____＼");
	CSgotoxy(x, y++);
	printf("                ＼:::/     /                  ~~                          ＼:::/    /                           ＼::/____/                          ＼::::/    /                           ＼:::/     /");
	CSgotoxy(x, y++);
	printf("                  ＼/_____/                                                 ＼/____/                              ~~                                  ＼:/____/                              ＼/____ /");
}
// 박스 그리기
void G_Box(int x, int y, int w, int h)
{
	CSgotoxy(x, y);
	printf("┌─");
	//printf("*");
	for (int i = 1; i <= w - 2; i++)
	{
		CSgotoxy(x + i * 2, y);
		printf("──");
		//printf("*");
	}
	CSgotoxy(x + (w - 1) * 2, y);
	printf("┐ ");
	//printf("*");

	for (int i = 1; i <= h - 2; i++)
	{
		CSgotoxy(x, y + i);
		printf("│ ");
		//printf("*");

		CSgotoxy(x + (w - 1) * 2, y + i);
		printf("│ ");
		//printf("*");
	}

	CSgotoxy(x, y + h - 1);
	printf("└─");
	//printf("*");
	for (int i = 1; i <= w - 2; i++)
	{
		CSgotoxy(x + i * 2, y + h - 1);
		printf("──");
		//printf("*");
	}
	CSgotoxy(x + (w - 1) * 2, y + h - 1);
	printf("┘ ");
	//printf("*");


}


// 서버 만들기
int N_ServerMake_Main()
{
	char system_str[100] = { 0 };
	sprintf(system_str, "mode con cols=%d lines=%d", 100, 30);
	system(system_str);

	// 화면 지우기
	G_Clean();

	// 포트 입력 받기 49152 ~ 65535
	int sPort = 30002;
	printf("49152 ~ 65535\n");
	printf("Server Port : ");
	scanf_s("%d",&sPort);
	if (sPort > 65535 || sPort < 49152)
	{
		if (sPort > 65535)
			sPort = 65535;
		else
			sPort = 49152;
		printf("포트 재설정 (%d)",sPort);
	}

	// 초기화
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Load WinSock 2.2 DLL Error\n");
		return 1;
	}


	// 서버 주소 초기화
	SOCKADDR_IN serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(sPort);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	printf("Server    IP : %s\n", inet_ntoa(serv_addr.sin_addr));
	printf("Server  Port : %d\n", sPort);

	// 서버 소켓 만들기
	SOCKET serv_sock;
	serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Socket Error\n");
		return 1;
	}

	// bind 후 listen
	if (bind(serv_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		printf("Bind Error\n");
		closesocket(serv_sock);
		WSACleanup();
		return 1;
	}
	if (listen(serv_sock, 5) == SOCKET_ERROR)
	{
		printf("Listen Error\n");
		closesocket(serv_sock);
		WSACleanup();
		return 1;
	}

	// 각 플레이어 포트 정하기 랜덤 4개
	int pPort[MAX_PLAYER] = { 0 };
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		pPort[i] = (rand() % 1638 + 4915) * 10 + 1 + i;
	}

	// 클라이언트와 연결 할 소켓 만들기
	SOCKADDR_IN clnt_addr;
	SOCKET clnt_sock;
	int size_clnt_addr = sizeof(clnt_addr);
	char data_send[20] = { 0 };
	//char recv_byte;
	char data_recv[20] = { 0 };

	char clnt_info[30] = { 0 };

	// 각 플레이어에게 포트 전달 & 연결 대기중
	printf("--PLAYER WAIT--\n");
	for (int pIndex, i = 0; i < MAX_PLAYER; i++)
	{
		// clnt_addr 초기화 
		memset(&clnt_addr, 0, sizeof(clnt_addr));
		
		printf("player wait... (%d / %d)\n", i, MAX_PLAYER);
		
		// 연결
		clnt_sock = accept(serv_sock, (SOCKADDR*)& clnt_addr, &size_clnt_addr);
		if (clnt_sock == INVALID_SOCKET)
			printf("%d_player Failed accept!!\n", i);


		// Clnt ip 출력
		sprintf(clnt_info, "%d %s", i, inet_ntoa(clnt_addr.sin_addr));
		printf("Clnt accept : %s\n", clnt_info);
		
		// 보낼 메시지 확인
		sprintf(data_send, "%d %d", i, pPort[i]);
		printf("send message : %s\n", data_send);
		
		// 포트 번호 전송
		send(clnt_sock, data_send, strlen(data_send) + 1, 0);
		// 확인 받기
		recv(clnt_sock, data_recv, sizeof(data_recv), 0);

		if (strcmp(data_recv, "%d %d", i, inet_ntoa(clnt_addr.sin_addr)) != 0)
		{
			closesocket(serv_sock);
			WSACleanup();
			return 1;
		}

		printf("\n");
		closesocket(clnt_sock);
		printf("closesocket : %s\n", clnt_info);
	}
	closesocket(serv_sock);

	/*
	
	전송 및 받기

	*/

	WSACleanup();
}

// 서버 들어가기
void N_ServerConnection()
{

	// 화면 지우기 
	G_Clean();

	WSADATA wsa_data;
	SOCKET clnt_sock;
	SOCKADDR_IN serv_addr;
	SOCKADDR_IN clnt_addr;

	char sAddr[15] = { 0 };
	int sPort;

	char data_send[20] = { 0 };

	// 서버 ip & port 입력 받기
	printf("서버 IP : ");
	scanf_s("%s", sAddr, 15);
	printf("포트 번호 : ");
	scanf_s("%s", data_send, 20);
	sPort = atoi(data_send);

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Load WinSock 2.2 DLL Error");
		exit(1);
	}

	clnt_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clnt_sock == INVALID_SOCKET)
	{
		printf("Socket Error");
		WSACleanup();
		exit(1);
	}

	// 서버 주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(sPort);
	serv_addr.sin_addr.s_addr = inet_addr(sAddr);
	//inet_pton(AF_INET, sAddr, &serv_addr.sin_addr);

	// Server Connect 
	if (connect(clnt_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		printf("Connection Error\n");
		closesocket(clnt_sock);
		WSACleanup();
		exit(1);
	}
	else
	{
		// 연결중
		// G_Connecting_Server();
		printf("Server Connect");
	}

	int nRcv;
	char data_recv[20] = { 0 };

	// 포트 번호 받기 (from Server
	nRcv = recv(clnt_sock, data_recv, sizeof(data_recv), 0);
	if (nRcv == SOCKET_ERROR || nRcv < 0)
	{
		printf("Receive Error\n");
		closesocket(clnt_sock);
		WSACleanup();
	}

	int pIndex;
	int pPort;

	pIndex = atoi(strtok(data_recv, " "));
	pPort = atoi(strtok(NULL, " "));

	// pIndex 와 pIP 보내기
	memset(&clnt_addr, 0, sizeof(clnt_addr));
	clnt_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);

	sprintf(data_send, "%d %s", pIndex, inet_ntoa(serv_addr.sin_addr));
	send(clnt_sock, data_send, strlen(data_send), 0);
	closesocket(clnt_sock);


	// 받은 포트 번호로 연결
	// 서버 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(pPort);
	serv_addr.sin_addr.s_addr = inet_addr(sAddr);
	//inet_pton(AF_INET, sAddr, &serv_addr.sin_addr);



	closesocket(clnt_sock);
	WSACleanup();
	while (TRUE)
	{
	}
}

// 게임 시작하기
void  P_Game_Start()
{
	while (TRUE)
	{
		// 그리기~
	}
}


// 게임 나가기
void P_GameExit()
{
	exit(1);
}
