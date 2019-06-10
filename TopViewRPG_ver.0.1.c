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

// 필수 변수 정의
typedef struct MouseInfo
{
	COORD MousePostion;
	int PressRight;
	int PressLeft;

}MouseInfo;

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

void MouseInput()
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


// 함수 정의
void B_ConsloeInit();

int P_Start();
void G_Title(int x, int y);
//void G_Menu();
void G_Box(int x, int y, int w, int h);
void N_ServerMake();
void N_ServerConnection();
void P_GameExit();


int main()
{
	// 기본 콘솔 정리
	B_ConsloeInit();
	// 마우스 입력 받기
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)MouseInput, NULL, 0, NULL);

	// 시작 화면
	StartMenu = P_Start();

	switch (StartMenu)
	{
	case 1:
		// -- 1. 서버 만들기 --
		N_ServerMake();
		break;

	case 2:
		// -- 2. 서버 들어가기 --
		N_ServerConnection();
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

	system("title Out Zombie");
	system(system_str);

}

// 시작화면
int P_Start()
{
	G_Title(15, 5);
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
void N_ServerMake()
{
	// 초기화
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("Load WinSock 2.2 DLL Error\n");
		return 1;
	}

	// 포트 입력 받기
	int sPort = 30002;

	// 서버 주소 초기화
	SOCKADDR_IN serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(sPort);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);


	SOCKET listen_sock;
	listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET)
	{
		WSACleanup();
		printf("Socket Error\n");
		return 1;
	}


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

	

	while (TRUE)
	{
		exit(1);
	}
}

// 서버 들어가기
void N_ServerConnection()
{
	while (TRUE)
	{
		exit(1);
	}
}

// 게임 나가기
void P_GameExit()
{
	exit(1);
}
