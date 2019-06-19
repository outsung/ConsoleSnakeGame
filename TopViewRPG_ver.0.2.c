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


// ucrtbase.dll 오류
// vcruntime140 오류

// 필수 메크로 정의
#define MAX_PLAYER 5

#define VK_0 0x30
#define VK_1 0x31
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44


// 필수 변수 정의

CRITICAL_SECTION cs;

typedef struct Serv_Param
{
	SOCKET _Socket;
	int ID;

}Serv_Param;

typedef struct Map
{
	FILE* pMap;
	int Width;
	int Height;
}Map;

typedef struct MouseInfo
{
	COORD MousePostion;
	int PressRight;
	int PressLeft;

}MouseInfo;

typedef struct PlayerPosition
{
	double X;
	double Y;

}PlayerPosition;

MouseInfo UserMouse = { 0 };

// 서버가 받을 정보가 있는지


PlayerPosition Player[MAX_PLAYER] = { 0 };
int PlayerID;

// 몇번 보냈는지 & 받았는지 확인 하는 변수
int pSendPing = 0;
int pRecvPing = 0;

// 시작 화면에서 선택 한거 
int StartMenu;

// 화면 해상도
int ScreenWidth, ScreenHeight;

// 접속한 각 플레이어에게 전한 포트
int pPort[MAX_PLAYER] = { 0 };

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

void T_KeyInput()
{
	while (TRUE)
	{
		if (IsKeyDown(VK_W))
		{
			if (Player[PlayerID].Y <= 1)
				;
			else
				Player[PlayerID].Y -= 0.1;
		}
		if (IsKeyDown(VK_D))
		{
			if (Player[PlayerID].X >= ScreenWidth - 2)
				;
			else
				Player[PlayerID].X += 0.2;
		}
		if (IsKeyDown(VK_A))
		{
			//Player[PlayerID].X -= 2;
			if (Player[PlayerID].X <= 2)
				;
			else
				Player[PlayerID].X -= 0.2;
		}
		if (IsKeyDown(VK_S))
		{
			if (Player[PlayerID].Y >= ScreenHeight - 1)
				;
			else
				Player[PlayerID].Y += 0.1;
		}

		// Speed 차이
		Sleep(3);
	}
}

void CSchange(int x, int y)
{
	char system_str[35] = { 0 };

	sprintf_s(system_str, 35, "mode con cols=%d lines=%d", x, y);
	system(system_str);
}

void SetColor(unsigned short color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void G_Clean()
{
	COORD sCoord = { 0,0 };
	DWORD dw;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', ScreenWidth * ScreenHeight, sCoord, &dw);
}

// Clnt
void T_N_cSend(void* _pArgs)
{
	/*
		Send 내용
		{
			PlayerID
			player position
			mouseInfo
		}
	*/

	SOCKET cSocket = *(SOCKET*)_pArgs;

	char Data_Send[100] = { 0 };
	int Data_SendSize;

	while (TRUE)
	{
		Data_Send[0] = '\0';

		Sleep(3);
		sprintf_s(Data_Send, 100, "%d %d %d ", PlayerID, (int)Player[PlayerID].X, (int)Player[PlayerID].Y);
		//UserMouse.MousePostion.X, UserMouse.MousePostion.Y);

		Data_SendSize = strlen(Data_Send);
		send(cSocket, Data_Send, Data_SendSize, 0);
		//printf("%s", Data_Send);

		pSendPing++;
	}

	//sprintf(system_str, "mode con cols=%d lines=%d", nWidth / FsizeX, nHeight / FsizeY);
	//send(cSocket, data_send, strlen(data_send), 0);

}
void T_N_cRecv(void* _pArgs)
{
	/*
	Secv 내용
	{
		0
		player position x y
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
	char* temp = "";

	int Data_RecvSize;

	int check;

	while (TRUE)
	{
		Data_Recv[0] = '\0';
		Data_RecvSize = recv(cSocket, Data_Recv, sizeof(Data_Recv), 0);
		//printf("aaaaaaaaaaaA");
		pRecvPing++;

		if (Data_RecvSize <= 0)
		{
			pRecvPing = -10000;
			break;
		}
		//CSgotoxy(0,1);
		//printf("---------%s------", Data_Recv);
		//printf("bbbbbbbbbbbbB");

		check = atoi(strtok_s(Data_Recv, " ", &temp));
		//sprintf_s(Data_Recv, 1024, temp);
		/*
		if (check != PlayerID)
			return;
			*/
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			if (i == PlayerID)
			{
				strtok_s(NULL, " ", &temp);
				strtok_s(NULL, " ", &temp);
			}
			else
			{
				Player[i].X = atoi(strtok_s(NULL, " ", &temp));
				Player[i].Y = atoi(strtok_s(NULL, " ", &temp));
			}
		}
		//printf("ccccccccccccC");
		Sleep(3);
	}

	printf("서버에서 연결 끊김\n");
	exit(1);
}



// Serv                                                                                    
void T_N_sSend(void* _pArgs)
{/*
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


	Serv_Param temp = *(Serv_Param*)_pArgs;


	SOCKET sSocket = temp._Socket;

	char Data_Send[1024] = { 0 };
	int Data_SendSize;


	while (TRUE)
	{
		// 배열 지우기
		Data_Send[0] = '\0';

		// 사람 수 만큼 플레이어 좌표 배열에 대입
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			sprintf_s(Data_Send,1024, "%s%d %d ", Data_Send, (int)Player[i].X, (int)Player[i].Y);
		}
		Data_SendSize = strlen(Data_Send);
		send(sSocket, Data_Send, Data_SendSize, 0);


		//Sleep(333);

		printf("서버 보냄\n");
	}
	*/
}
void T_N_sRecv(void* _pArgs)
{
	/*
	Send 내용
	{
		ID
		player position
		mouseInfo
	}
	*/

	Serv_Param temp_ = *(Serv_Param*)_pArgs;

	SOCKET sSocket = temp_._Socket;
	int pID = temp_.ID;

	char Data_Recv[100] = { 0 };
	char* Strtok_temp = 0;
	int Data_RecvSize;
	int check;


	printf("%d 쓰레드 시작\n", pID);

	char Data_Send[1024] = { 0 };
	int Data_SendSize;



	while (TRUE)
	{
		Data_Recv[0] = '\0';
		Data_RecvSize = recv(sSocket, Data_Recv, sizeof(Data_Recv), 0);
		printf("받은 데이터 %s \n", Data_Recv);

		if (Data_RecvSize <= 0)
			return;

		check = atoi(strtok_s(Data_Recv, " ", &Strtok_temp));
		if (check != pID)
		{
			printf("%d != %d 아이디 안맞음\n", check, pID);
			return;
		}

		Player[pID].X = atoi(strtok_s(NULL, " ", &Strtok_temp));
		Player[pID].Y = atoi(strtok_s(NULL, " ", &Strtok_temp));

		printf("------서버 받음 %d -------\n", pID);

		Data_Send[0] = '\0';
		sprintf_s(Data_Send, 1024, "%d ", pID);
		// 사람 수 만큼 플레이어 좌표 배열에 대입
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			sprintf_s(Data_Send, 1024, "%s%d %d ", Data_Send, (int)Player[i].X, (int)Player[i].Y);
		}
		printf("%s -> %d 보낼예정\n", Data_Send, pID);
		Data_SendSize = strlen(Data_Send);
		send(sSocket, Data_Send, Data_SendSize, 0);


		//Sleep(333);

		printf("서버 보냄 %d \n", pID);
	}
	printf("클라이언트에서 연결 끊김\n");
	return;
}


// ver.0.2
void G_Home();
int P_Home();
// 함수 정의
void B_ConsloeInit();
void B_Init();
int P_Start();
void G_Title(int x, int y);
//void G_Menu();
void G_Box(int x, int y, int w, int h);
int N_ServerMake_Main();
void N_ServerConnection();
void P_GameExit();
void P_Game_Start();
void G_player();
void G_player_Info(int x, int y, int ID);
void G_player_Shape(int x, int y);
void G_Ping();

int main()
{
	// 기본 콘솔 정리
	B_ConsloeInit();

	// 마우스 입력 받기
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)T_MouseInput, NULL, 0, NULL);

	InitializeCriticalSection(&cs);


	// 변수 초기화
	B_Init();

	// 시작 화면
	StartMenu = P_Start();

	if (StartMenu == 3)
	{
		P_GameExit();
	}
	
	StartMenu = P_Home();

	switch (StartMenu)
	{
	case 1:

		break;
	default:
		break;
	}

	// 서버 선택 화면

	/*
	switch (StartMenu)
	{
	case 1:
		// -- 1. 서버 만들기 --
		//로딩중
		//N_ServerMake_Main();
		break;

	case 2:
		//-- 2. 게임 시작 --
		P_Home();
		//N_ServerConnection();
		//P_Game_Start();
		break;

	case 3:
		// -- 3. 게임 나가기 --
		P_GameExit();
		break;

	default:
		break;
	}
	*/

	P_GameExit();
	return 0;
}

// 콘솔 초기화
void B_ConsloeInit()
{
	CONSOLE_CURSOR_INFO cr;

	cr.dwSize = 1;
	cr.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cr);
	int FntSizeX, FntSizeY;

	int ConsoleWidth, ConsoleHeight;

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	CONSOLE_FONT_INFO Fn_;
	GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &Fn_);

	FntSizeX = Fn_.dwFontSize.X;
	FntSizeY = Fn_.dwFontSize.Y;

	ConsoleWidth = ScreenWidth / FntSizeX;
	ConsoleHeight = ScreenHeight / FntSizeY - 4;


	CSchange(ConsoleWidth, ConsoleHeight);
	/*
	char system_str[35] = { 0 };

	sprintf_s(system_str, 35, "mode con cols=%d lines=%d", ConsoleWidth, ConsoleHeight);
	system(system_str);
	*/

	system("title Out Zombie");


}

// 변수 초기화
void B_Init()
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		Player[i].X = rand() % 5 + 1;
		Player[i].Y = rand() % 5 + 1;
	}
}

// 시작화면
int P_Start()
{
	G_Title(15, 5);
	while (TRUE)
	{
		/*
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
		*/

		// 서버 들어가기 박스
		if ((UserMouse.MousePostion.X >= 149 && UserMouse.MousePostion.X <= 149 + 10 * 2) &&
			(UserMouse.MousePostion.Y >= 9 && UserMouse.MousePostion.Y <= 9 + 5))
		{
			if (UserMouse.PressLeft == 1)
				return 2;
			SetColor(14);
		}
		else SetColor(15);

		G_Box(149, 9, 10, 5);
		CSgotoxy(154, 11);
		printf("게임  시작");

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

// 홈
int P_Home()
{
	G_Clean();
	G_Home();
	while (TRUE)
	{
		// 첫번째
		if ((UserMouse.MousePostion.X >= 5 && UserMouse.MousePostion.X <= 5 + 45 * 2) &&
			(UserMouse.MousePostion.Y >= 7 && UserMouse.MousePostion.Y <= 7 + 10))
		{
			if (UserMouse.PressLeft == 1)
				return 1;
			SetColor(14);
		}
		else SetColor(15);
		G_Box(5, 7, 45, 10);


		// 두번째
		if ((UserMouse.MousePostion.X >= 5 && UserMouse.MousePostion.X <= 5 + 45 * 2) &&
			(UserMouse.MousePostion.Y >= 20 && UserMouse.MousePostion.Y <= 20 + 10))
		{
			if (UserMouse.PressLeft == 1)
				return 2;
			SetColor(14);
		}
		else SetColor(15);
		G_Box(5, 20, 45, 10);

		// 세번째
		if ((UserMouse.MousePostion.X >= 5 && UserMouse.MousePostion.X <= 5 + 45 * 2) &&
			(UserMouse.MousePostion.Y >= 33 && UserMouse.MousePostion.Y <= 33 + 10))
		{
			if (UserMouse.PressLeft == 1)
				return 3;
			SetColor(14);
		}
		else SetColor(15);
		G_Box(5, 33, 45, 10);
	}
}
// 홈
void G_Home()
{

	CSgotoxy(5, 2);
	printf("-- Server List --");

	G_Box(5, 7, 45, 10);
	G_Box(5, 20, 45, 10);
	G_Box(5, 33, 45, 10);

	CSgotoxy(110, 2);
	printf("-- Manual --");

	CSgotoxy(151, 2);
	printf("○");
	CSgotoxy(150, 3);
	printf("(  )");
	CSgotoxy(157, 2);
	printf("2 / 20");

	CSgotoxy(113, 4);
	printf("△ =  W");
	CSgotoxy(113, 6);
	printf("▽ =  S");
	CSgotoxy(113, 8);
	printf("◁ =  A");
	CSgotoxy(113, 10);
	printf("▷ =  D");
}


// 서버 만들기
int N_ServerMake_Main()
{
	CSchange(100, 50);
	/*
	char system_str[100] = { 0 };
	sprintf_s(system_str, 100, "mode con cols=%d lines=%d", 100, 30);
	system(system_str);
	*/
	// 화면 지우기
	G_Clean();

	// 포트 입력 받기 49152 ~ 65535
	int sPort = 30002;
	printf("49152 ~ 65535\n");
	printf("Server Port : ");
	scanf_s("%d", &sPort);
	fflush(stdin);
	if (sPort > 65535 || sPort < 49152)
	{
		if (sPort > 65535)
			sPort = 65535;
		else
			sPort = 49152;
		printf("포트 재설정 (%d)\n", sPort);
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

	printf("Server    IP : %s\n", inet_ntoa(serv_addr.sin_addr));
	printf("Server  Port : %d\n", sPort);

	// 각 플레이어 포트 정하기 랜덤 4개
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
	char check[20] = { 0 };
	printf("--PLAYER WAIT--\n");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		// clnt_addr 초기화 
		memset(&clnt_addr, 0, sizeof(clnt_addr));

		printf("player wait... (%d / %d)\n", i, MAX_PLAYER);

		// 연결
		clnt_sock = accept(serv_sock, (SOCKADDR*)& clnt_addr, &size_clnt_addr);
		if (clnt_sock == INVALID_SOCKET)
			printf("%d_player Failed accept!!\n", i);


		// Clnt ip 출력
		sprintf_s(clnt_info, 30, "%d %s", i, inet_ntoa(clnt_addr.sin_addr));
		printf("연결해옴 : %s\n", clnt_info);

		// 보낼 메시지 확인
		sprintf_s(data_send, 20, "%d %d", i, pPort[i]);
		printf("send message : %s\n", data_send);

		// 포트 번호 전송
		send(clnt_sock, data_send, strlen(data_send) + 1, 0);
		// 확인 받기
		recv(clnt_sock, data_recv, sizeof(data_recv), 0);


		check[0] = '\0';
		sprintf_s(check, 20, "%d %s", i, inet_ntoa(clnt_addr.sin_addr));
		/*
		if (strcmp(data_recv, check) != 0)
		{
			closesocket(serv_sock);
			WSACleanup();
			return 1;
		}
		*/
		closesocket(clnt_sock);
		printf("closesocket : %s\n\n", clnt_info);
	}
	closesocket(serv_sock);

	// 포트로 연결 후 쓰레드 호출
	SOCKET clnt_sock_[MAX_PLAYER];

	Serv_Param temp;

	printf("--PLAYER Cenneted--\n");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		printf("player wait... (%d / %d)\n", i, MAX_PLAYER);


		// 서버 소켓 포트 설정
		serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //-----------------------------------------------------------------------------------------------------------------------
		if (serv_sock == INVALID_SOCKET)
		{
			WSACleanup();
			printf("Socket Error\n");
			return 1;
		}

		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(pPort[i]);
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

		printf("open Port : %d\n", pPort[i]);
		// 서버와 연결 받을 클라이언트 소켓을 받을 소켓 생성
		memset(&clnt_addr, 0, sizeof(clnt_addr));
		clnt_sock_[i] = accept(serv_sock, (SOCKADDR*)& clnt_addr, &size_clnt_addr);
		if (clnt_sock_[i] == INVALID_SOCKET)
			printf("%d_player Failed accept!!\n", i);
		printf("연결 Port : %d\n", pPort[i]);

		temp._Socket = clnt_sock_[i];
		temp.ID = i;

		//_beginthreadex(NULL, 0, (_beginthreadex_proc_type)T_N_sSend, temp, 0, NULL);
		_beginthreadex(NULL, 0, (_beginthreadex_proc_type)T_N_sRecv, (void*)& temp, 0, NULL);
		/*
		T_N_sSend(clnt_sock,i);
		T_N_sRecv(clnt_sock,i);
		*/

		closesocket(serv_sock);
	}

	while (TRUE)
		if (IsKeyDown(VK_END))
			break;

	WSACleanup();
	for (int i = 0; i < MAX_PLAYER; i++)
		closesocket(clnt_sock_[i]);
	return 0;
}

// 서버 들어가기
void N_ServerConnection()
{

	// 화면 지우기 
	G_Clean();
	//system("cls");
	CSchange(100, 50);

	WSADATA wsa_data;
	SOCKET clnt_sock;
	SOCKADDR_IN serv_addr;
	//SOCKADDR_IN clnt_addr;

	char sAddr[15] = { 0 };
	int sPort;

	char data_send[20] = { 0 };
	CSgotoxy(0, 0);
	// 서버 ip & port 입력 받기
	printf("서버 IP : ");
	scanf_s("%s", sAddr, 15);
	fflush(stdin);
	printf("포트 번호 : ");
	scanf_s("%s", data_send, 20);
	sPort = atoi(data_send);
	fflush(stdin);

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Load WinSock 2.2 DLL Error");
		fflush(stdin);
		exit(1);
	}

	clnt_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clnt_sock == INVALID_SOCKET)
	{
		printf("Socket Error");
		WSACleanup();
		fflush(stdin);
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
		fflush(stdin);
		exit(1);
	}
	else
	{
		// 연결중
		// G_Connecting_Server();
		printf("Server Connect\n");
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
	int CpPort;

	char* Strtok_temp = 0;

	pIndex = atoi(strtok_s(data_recv, " ", &Strtok_temp));
	CpPort = atoi(strtok_s(NULL, " ", &Strtok_temp));
	printf("%d 플레이어 포트:%d\n", pIndex + 1, CpPort);
	// pIndex 와 pIP 보내기
	/*
	memset(&clnt_addr, 0, sizeof(clnt_addr));
	clnt_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	*/
	sprintf_s(data_send, 20, "%d %s", pIndex, inet_ntoa(serv_addr.sin_addr));
	send(clnt_sock, data_send, strlen(data_send), 0);
	closesocket(clnt_sock);


	printf("%d Press-----\n", pIndex);
	while (TRUE)
	{
		if (pIndex == 0)
		{
			if (IsKeyDown(VK_0))
				break;

		}
		if (pIndex == 1)
		{

			if (IsKeyDown(VK_1))
				break;
		}
	}


	// 받은 포트 번호로 연결
	// 서버 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(CpPort);
	serv_addr.sin_addr.s_addr = inet_addr(sAddr);
	//inet_pton(AF_INET, sAddr, &serv_addr.sin_addr);

	clnt_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clnt_sock == INVALID_SOCKET)
	{
		printf("Socket Error");
		WSACleanup();
		fflush(stdin);
		exit(1);
	}

	// 새로운 포트로 연결
	if (connect(clnt_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		printf("Connection Error\nConnection Error\nConnection Error\nConnection Error\nConnection Error\nConnection Error\nConnection Error\nConnection Error\nConnection Error\n");
		closesocket(clnt_sock);
		WSACleanup();
		fflush(stdin);
		exit(0);
	}
	else
	{
		// 연결중
		// G_Connecting_Server();
		printf("Server Connect");
	}

	PlayerID = pIndex;

	// 쓰레드로 출력
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)T_N_cSend, (void*)& clnt_sock, 0, NULL);
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)T_N_cRecv, (void*)& clnt_sock, 0, NULL);
	/*
	T_N_cSend(clnt_sock);
	T_N_cRecv(clnt_sock);
	*/



	CSchange(ScreenWidth, ScreenHeight);
	P_Game_Start();

	closesocket(clnt_sock);
	WSACleanup();
}

void a()
{

}

// 게임 시작하기
void P_Game_Start()
{
	G_player();
}

void G_player()
{
	// 키보드 입력 받기
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)T_KeyInput, NULL, 0, NULL);
	while (TRUE)
	{
		G_Clean();
		G_Ping();
		for (int i = 0; i < MAX_PLAYER; i++)
		{
			G_player_Info((int)Player[i].X, (int)Player[i].Y, i);
			G_player_Shape((int)Player[i].X, (int)Player[i].Y);
		}
		Sleep(33);

		if (IsKeyDown(VK_END))
			break;
	}
}

void G_player_Info(int x, int y, int ID)
{
	CSgotoxy(x, y - 1);
	printf("%dp", ID);
}

void G_player_Shape(int x, int y)
{
	CSgotoxy(x, y);
	printf("◎");
}

void G_Ping()
{
	CSgotoxy(0, 0);
	printf("%dP_ping : %d(S) , %d(R)", PlayerID, pSendPing, pRecvPing);
}

// 게임 나가기
void P_GameExit()
{
	CSgotoxy(0, 0);
	fflush(stdin);
	DeleteCriticalSection(&cs);

	exit(1);
}
