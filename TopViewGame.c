#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <malloc.h>

char temp_1[100] = { 0, };
char temp_2[100] = { 0, };
char temp_3[100] = { 0, };

CRITICAL_SECTION cs;

#define PI 3.1415926535

HANDLE Screen[2];
HANDLE Background;

int ScreenIndex = 1;

typedef struct playerPostion
{
	double X;
	double Y;

} playerPostion;

COORD MousePostion = { 50,50 };
playerPostion KeyPostion = { 50.0, 50.0 };

int nWidth;
int nHeight;

int center_x = 0;
int center_y = 0;

int Background_W = 300;
int Background_H = 400;

//사용할 키보드 가상키 정의하기
//참고 : https://m.blog.naver.com/power2845/50143021565
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

int IsKeyDown(int key)
{
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}



void ScreenInit()
{
	CONSOLE_CURSOR_INFO cr;

	Screen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	Screen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	Background = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	cr.dwSize = 1;
	cr.bVisible = FALSE;

	SetConsoleCursorInfo(Screen[0], &cr);
	SetConsoleCursorInfo(Screen[1], &cr);


	MoveWindow(Screen[0], 0, 0, nWidth, nHeight, 0);
	MoveWindow(Screen[1], 0, 0, nWidth, nHeight, 0);

	COORD Background_size = { Background_W,Background_H };
	SetConsoleScreenBufferSize(Background, Background_size);


	COORD sCoord = { 0,0 };
	DWORD dw;

	FillConsoleOutputCharacter(Background, ' ', Background_W * Background_H, sCoord, &dw);

	//SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cr);
}


void ScreenClear()
{
	COORD sCoord = { 0,0 };
	DWORD dw;
	FillConsoleOutputCharacter(Screen[ScreenIndex], ' ', 300 * 200, sCoord, &dw);

}

void BackgroundPrint(int x, int y, char* str)
{
	COORD CursorPosition = { (SHORT)x, (SHORT)y };
	DWORD dw;

	SetConsoleCursorPosition(Background, CursorPosition);
	WriteConsole(Background, str, strlen(str), &dw, NULL);
}


void ScreenPrint(int x, int y, char* str)
{
	EnterCriticalSection(&cs);
	COORD CursorPosition = { (SHORT)x, (SHORT)y };
	DWORD dw;

	SetConsoleCursorPosition(Screen[ScreenIndex], CursorPosition);
	WriteConsole(Screen[ScreenIndex], str, strlen(str), &dw, NULL);
	LeaveCriticalSection(&cs);
}

void ScreenFlipping()
{
	Sleep(33);

	SetConsoleActiveScreenBuffer(Screen[ScreenIndex]);
	ScreenIndex = !ScreenIndex;
	SetConsoleActiveScreenBuffer(Screen[ScreenIndex]);
}

void SetColor(unsigned short color)
{
	SetConsoleTextAttribute(Screen[ScreenIndex], color);
}


void bullet()
{
	int x1 = center_x, y1 = center_y;


	clock_t old_time, new_time;
	int bullet_speed = 10;


	int x2 = MousePostion.X, y2 = MousePostion.Y;


	int addX, addY = 0;
	int counter = 0;

	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx < 0)
	{
		addX = -1;
		dx = -dx;
	}
	else
	{
		addX = 1;
	}

	if (dy < 0)
	{
		addY = -1;
		dy = -dy;
	}
	else
	{
		addY = 1;
	}

	int x = x1;
	int y = y1;

	if (dx >= dy)
	{
		for (int i = 0; i < dx; i++)
		{
			x += addX;
			counter += dy;

			if (counter >= dx)
			{
				y += addY;
				counter -= dx;
			}

			old_time = clock();
			while (TRUE)
			{
				ScreenPrint(x, y, "*");
				new_time = clock();
				if (new_time - old_time > bullet_speed)
				{
					break;
				}

			}
		}
	}
	else
	{

		for (int i = 0; i < dy; i++)
		{
			y += addY;
			counter += dx;

			if (counter >= dy)
			{
				x += addX;
				counter -= dy;
			}

			old_time = clock();
			while (TRUE)
			{
				ScreenPrint(x, y, "*");
				new_time = clock();
				if (new_time - old_time > bullet_speed)
				{
					break;
				}

			}
		}
	}

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
			MousePostion.X = rec.Event.MouseEvent.dwMousePosition.X;
			MousePostion.Y = rec.Event.MouseEvent.dwMousePosition.Y;

			//printf("A");// 커서의 위치를 왼쪽 버튼이 눌려진 위치로 이동합니다.

			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)// 왼쪽 버튼이 눌렸을 경우,
			{

				if (time - Attacked_time > 333)// 총 딜레이
				{
					_beginthreadex(NULL, 0, (_beginthreadex_proc_type)bullet, NULL, 0, NULL);
					Attacked_time = clock();
				}
			}
		}
	}
}

void KeyInput()
{
	while (TRUE)
	{
		if (IsKeyDown(VK_W))
		{
			KeyPostion.Y -= 0.1;
		}
		if (IsKeyDown(VK_D))
		{
			KeyPostion.X += 0.2;
		}
		if (IsKeyDown(VK_A))
		{
			KeyPostion.X -= 0.2;
		}
		if (IsKeyDown(VK_S))
		{
			KeyPostion.Y += 0.1;
		}
		Sleep(3);
	}
}






void S_print_background()
{
	BackgroundPrint(60, 60, "□□□□□□");
	BackgroundPrint(60, 61, "□        □");
	BackgroundPrint(60, 62, "□        □");
	BackgroundPrint(60, 63, "□        □");
	BackgroundPrint(60, 64, "□□    □□");

	BackgroundPrint(20, 60, "□□□□□□");
	BackgroundPrint(20, 61, "□        □");
	BackgroundPrint(20, 62, "□        □");
	BackgroundPrint(20, 63, "□        □");
	BackgroundPrint(20, 64, "□□    □□");
	
	BackgroundPrint(80, 60, "□□□□□□");
	BackgroundPrint(80, 61, "□        □");
	BackgroundPrint(80, 62, "□        □");
	BackgroundPrint(80, 63, "□        □");
	BackgroundPrint(80, 64, "□□    □□");
}

void S_print_palyer()
{
	/*
	"  ■■■  "
	"■      ■"
	"■  ■  ■"
	"■      ■"
	"  ■■■  "
	*/
	char temp_1[] = "■■■";
	char temp_2[] = "■  ■";
	char temp_3[] = "■■■";


	ScreenPrint(center_x - 2, center_y - 1, temp_1);
	ScreenPrint(center_x - 2, center_y, temp_2);
	ScreenPrint(center_x - 2, center_y + 1, temp_3);

}

void S_print_mouse()
{
	/*
	char temp_1[] = "┌─┬─┐";
	char temp_2[] = "├─┼─┤";
	char temp_3[] = "└─┴─┘";

	ScreenPrint(MousePostion.X - 2, MousePostion.Y - 1, temp_1);
	ScreenPrint(MousePostion.X - 2, MousePostion.Y, temp_2);
	ScreenPrint(MousePostion.X - 2, MousePostion.Y + 1, temp_3);
	*/
}

void S_print_flash(double angle)
{
	//das = distance * cos(angle);
	//
}




/*
int move_rect()
{
	
}
*/




void Render()
{
	int x = (int)KeyPostion.X - center_x;
	int y = (int)KeyPostion.Y - center_y;
	int result = 0;

	PCHAR_INFO buffer = (PCHAR_INFO)malloc(240 * 50);
	
	COORD      buffer_size = { 240, 50 };
	COORD      buffer_index = { 0, 0 };
	SMALL_RECT read_rect = { x, y, x + 240 - 1, y + 50 - 1 };
	SMALL_RECT write_rect = { 0, 0, 0 + 240 - 1, 0 + 50 - 1 };

	result = ReadConsoleOutput(Background, buffer, buffer_size, buffer_index, &read_rect)
		&& WriteConsoleOutput(Screen[ScreenIndex], buffer, buffer_size, buffer_index, &write_rect);

	free(buffer);


	// 배경 불러오기


	sprintf(temp_3, "확인%d", result);

	//ScreenPrint(KeyPostion.X, KeyPostion.Y, "■");

	//S_print_mouse();

	//각도 출력
	ScreenPrint(0, 0, temp_1);

	// player 출력
	S_print_palyer();


	//해상도 출력
	ScreenPrint(0, 2, temp_2);

	ScreenPrint(0, 5, temp_3);

	ScreenFlipping();
	ScreenClear();
}

void Update()
{
	double res = 0;
	double R_angle = atan2(MousePostion.Y * -1 + center_y, MousePostion.X - center_x);

	res = R_angle * 180 / PI;

	sprintf(temp_1, "x = %d, y = %d => 각도 : %lf", MousePostion.X - center_x, MousePostion.Y * -1 + center_y, res);


	sprintf(temp_2, "player 좌표 %lf, %lf", KeyPostion.X, KeyPostion.Y);

}



void Inin()
{
}


//콘솔 크기 변경 및 이동
void Console_Init()
{
	//(X,Y,) = (좌,상) 
	//참고 : https://changmyeong.tistory.com/32

	int FsizeX, FsizeY;
	char system_str[300] = { 0, };

	system("title TopViewGame");

	HWND hOut = FindWindow(NULL, "TopViewGame");

	nWidth = GetSystemMetrics(SM_CXSCREEN);
	nHeight = GetSystemMetrics(SM_CYSCREEN);

	CONSOLE_FONT_INFO Fn_;
	GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &Fn_);

	FsizeX = Fn_.dwFontSize.X;
	FsizeY = Fn_.dwFontSize.Y;

	//sprintf(temp_3, "글꼴 크기 (%d, %d)\n버퍼 크기 (%d, %d)", FsizeX, FsizeY, nWidth / FsizeX, nHeight / FsizeY);


	center_x = nWidth / FsizeX / 2;
	center_y = nHeight / FsizeY / 2;


	sprintf(system_str, "mode con cols=%d lines=%d", nWidth / FsizeX, nHeight / FsizeY);

	system(system_str);

	//MoveWindow(hOut, X, Y, nWidth, nHeight, 0);

}



int main()
{
	InitializeCriticalSection(&cs);

	Console_Init();
	ScreenInit();
	Inin();


	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)MouseInput, NULL, 0, NULL);
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)KeyInput, NULL, 0, NULL);


	S_print_background();

	//게임 시작 전
	while (1)
	{

		if (IsKeyDown(VK_ESCAPE))
		{
			break;
		}

		Update();
		Render();


	}
	//게임 시작

	//게임 종료
	CloseHandle(Screen[0]);
	CloseHandle(Screen[1]);
	DeleteCriticalSection(&cs);
	return 0;
}
