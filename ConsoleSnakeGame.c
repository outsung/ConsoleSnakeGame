#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <math.h>
#include <time.h>
#include <locale.h>


int IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

typedef struct vector2
{
	int x;
	int y;

}vector2;

typedef struct snake
{
	int x;
	int y;

}snake;

void CSgotoxy(int X, int Y)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD initial = { X,Y };

	SetConsoleCursorPosition(hOut, initial);

}

enum { START, SETTING, EXIT};

enum { UP, DOWN, LEFT, RIGHT };

enum { DELAY, STAR };

//enum { ●, △, ▲ };  print_head
//		 0   1   2
//enum { ★, ◆, ♠, ♥, ♣ }; print_item
//		 0   1   2   3   4
//enum { ○, ●, ◎, ⊙, ▣, ◇, ◆, ◈, ♡, ♥, ♧, ♣, ♤, ♠}; print_body
//		 0   1   2   3   4   5   6   7   8   9   10  11  12  13 

int speed = 100;
int speed_delay = 1;
int speed_point = 1;

double game_time = 0;

int menu = 0;

int way = DOWN;

int head_check = 0;
int item_check = 0;
int body_check = 0;

snake main_snake[150] = { { 50,15 },{ 50,14 },{ 50,13 } };
snake item[2] = { {0,0},{0,0} };

int tail = 2;

int score = 0;


UINT __stdcall Thread(void *arg)
{
	while (1)
	{
		clock_t before;
		before = clock();
		if (way == UP || way == DOWN)
		{
			if (IsKeyDown(VK_LEFT))
				way = LEFT;
			if (IsKeyDown(VK_RIGHT))
				way = RIGHT;
		}
		else if (way == RIGHT || way == LEFT)
		{
			if (IsKeyDown(VK_UP))
				way = UP;
			if (IsKeyDown(VK_DOWN))
				way = DOWN;
		}
		game_time += (double)(clock() - before) / CLOCKS_PER_SEC;
		
	}
}


void print_item(void)
{
	//enum { ●, △, ▲ };  print_head
	//		 0   1   2
	//enum { ★, ◆, ♠, ♥, ♣ }; print_item
	//		 0   1   2   3   4
	//enum { ○, ●, ◎, ⊙, ▣, ◇, ◆, ◈, ♡, ♥, ♧, ♣, ♤, ♠}; print_body
	//		 0   1   2   3   4   5   6   7   8   9   10  11  12  13 

	switch (item_check)
	{
	case 0:
		printf("★");
		break;
	case 1:
		printf("◆");
		break;
	case 2:
		printf("♠");
		break;
	case 3:
		printf("♥");
		break;
	case 4:
		printf("♣");
		break;

	}

}



void star_random(int x)
{
	
	re_rand:

	item[x].x = ((rand() % 47) + 1) * 2;
	item[x].y = ((rand() % 14) + 1) * 2;

	if (x == 0)
	{
		if ((item[x].x == item[1].x) && (item[x].y == item[1].y))
		{
			goto re_rand;
		}
	}
	else
	{
		if ((item[x].x == item[0].x) && (item[x].y == item[0].y))
		{
			goto re_rand;
		}
	}



	for (int i = 0; i < tail; i++)
	{
		if ( (main_snake[i].x == item[x].x) && (main_snake[i].y == item[x].y) )
		{
			goto re_rand;
		}

	}

	CSgotoxy(item[x].x, item[x].y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);
	print_item();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


}


void initial_setting(void)
{
	system("color 0F");
	system("mode con: cols=163 lines=37");
	system("title 뱀 게임^^^^7");
	_beginthreadex(NULL, 0, Thread, 0, 0, NULL);



	CONSOLE_CURSOR_INFO CurInfo;


	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);

	srand((unsigned int)time(NULL));
	
}


void print_starting(void)
{
	DWORD  dw, dwmode;
	INPUT_RECORD rec;
	

	printf("                                     ___                ____               ___                  ___                    ____                  \n");
	printf("                                    /  /＼             /  / ＼            /  /＼               /  /＼                 /  / ＼                \n");
	printf("                                   /  /: :＼          /  /: :|           /  /: :＼            /  / :/                /  / : :＼              \n");
	printf("                                  /__/:/＼: ＼       /  /: |:|          /  /: /＼:＼         /  / :/                /  / : /＼:＼            \n");
	printf("                               ___＼_＼:＼ ＼:＼    /  /: /|:|__       /  /:: ＼ ＼:＼      /  / ::＼________      /  / :  ＼ ＼:＼          \n");
	printf("                              /___/＼  ＼:＼ ＼:＼ /__/ :/ |:| /＼    /__/: /＼:＼_＼:＼   /  / :/＼    :::::＼   /__/ : /＼:＼ ＼:＼        \n");
	printf("                              ＼  ＼:＼  ＼:＼_＼/ ＼_＼/  |:|/ :/    ＼_＼/   ＼:＼/: /  /__/ :/~~| :|~~~~~~~~   ＼  ＼:＼ ＼:＼_＼/        \n");
	printf("                                ＼  ＼:＼__＼:＼       |   |:/ :/           ＼___＼:: /   ＼_＼/   | :|             ＼  ＼:＼ ＼:＼          \n");
	printf("                                  ＼  ＼:＼ /:/        |___|: :/             /   / : /         |   | :|               ＼  ＼:＼_＼/          \n");
	printf("                                    ＼  ＼ ::/         /__/ : /             /___/  :/          |___| :|                 ＼  ＼:＼            \n");
	printf("                                      ＼__＼/          ＼_＼_/              ＼___＼/            ＼__＼|                   ＼__＼/            \n");
	  
	CSgotoxy(64, 17);
	printf("┌──────────────┐  ");
	CSgotoxy(64, 18);
	printf("│          게임시작          │  ");
	CSgotoxy(64, 19);
	printf("└──────────────┘  ");

	CSgotoxy(64, 21);
	printf("┌──────────────┐  ");
	CSgotoxy(64, 22);
	printf("│          환경설정          │  ");
	CSgotoxy(64, 23);
	printf("└──────────────┘  ");


	CSgotoxy(64, 25);
	printf("┌──────────────┐  ");
	CSgotoxy(64, 26);  
	printf("│          게임종료          │  ");
	CSgotoxy(64, 27);
	printf("└──────────────┘  ");


	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwmode) | ENABLE_MOUSE_INPUT);

	while (1)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dw);
		if (rec.EventType == MOUSE_EVENT)
		{
			if ((rec.Event.MouseEvent.dwMousePosition.X > 64 && rec.Event.MouseEvent.dwMousePosition.X < 95) && (rec.Event.MouseEvent.dwMousePosition.Y > 16 && rec.Event.MouseEvent.dwMousePosition.Y < 20))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0E);
				CSgotoxy(64, 17);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 18);
				printf("│          게임시작          │  ");
				CSgotoxy(64, 19);
				printf("└──────────────┘  ");

				CSgotoxy(0, 30);

				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					menu = START;
					return;
				}
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				CSgotoxy(64, 17);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 18);
				printf("│          게임시작          │  ");
				CSgotoxy(64, 19);
				printf("└──────────────┘  ");

				CSgotoxy(0, 30);

			}



			if ((rec.Event.MouseEvent.dwMousePosition.X > 64 && rec.Event.MouseEvent.dwMousePosition.X < 95) && (rec.Event.MouseEvent.dwMousePosition.Y > 20 && rec.Event.MouseEvent.dwMousePosition.Y < 24))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				CSgotoxy(64, 21);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 22);
				printf("│          환경설정          │  ");
				CSgotoxy(64, 23);
				printf("└──────────────┘  ");

				CSgotoxy(0, 30);

				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					menu = SETTING;
					return;
				}


				
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				CSgotoxy(64, 21);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 22);
				printf("│          환경설정          │  ");
				CSgotoxy(64, 23);
				printf("└──────────────┘  ");

				CSgotoxy(0, 30);

			}





			if ((rec.Event.MouseEvent.dwMousePosition.X > 64 && rec.Event.MouseEvent.dwMousePosition.X < 95) && (rec.Event.MouseEvent.dwMousePosition.Y > 24 && rec.Event.MouseEvent.dwMousePosition.Y < 28))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				CSgotoxy(64, 25);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 26);
				printf("│          게임종료          │  ");
				CSgotoxy(64, 27);
				printf("└──────────────┘  ");

				CSgotoxy(0, 30);

				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					menu = EXIT;
					return;
				}


			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				CSgotoxy(64, 25);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 26);
				printf("│          게임종료          │  ");
				CSgotoxy(64, 27);
				printf("└──────────────┘  ");

				CSgotoxy(0, 30);
			}


		}


	}
	
}



void print_wall(void)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);

	CSgotoxy(0, 0);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	for (int i = 1; i < 29; i++)
	{
		CSgotoxy(0, i);
		printf("■                                                                                                ■");
	}
	CSgotoxy(0, 29);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);

}

void print_title(vector2 start_point)
{
	// x = 108
	// y = 2
	
	CSgotoxy(start_point.x, start_point.y);
	printf(" ######   ##    ##     ###     ##    ##  ########  ");
	CSgotoxy(start_point.x, start_point.y + 1);
	printf("##    ##  ###   ##    ## ##    ##   ##   ##        ");
	CSgotoxy(start_point.x, start_point.y + 2);
	printf("##        ####  ##   ##   ##   ##  ##    ##        ");
	CSgotoxy(start_point.x, start_point.y + 3);
	printf(" ######   ## ## ##  ##     ##  #####     ######    ");
	CSgotoxy(start_point.x, start_point.y + 4);
	printf("      ##  ##  ####  #########  ##  ##    ##        ");
	CSgotoxy(start_point.x, start_point.y + 5);
	printf("##    ##  ##   ###  ##     ##  ##   ##   ##        ");
	CSgotoxy(start_point.x, start_point.y + 6);
	printf(" ######   ##    ##  ##     ##  ##    ##  ########  ");
}


void print_continue(vector2 start_point)
{
	// x = 49 
	// y = 3

	CSgotoxy(start_point.x, start_point.y);
	printf("                       #       #                                 ###  ");
	CSgotoxy(start_point.x, start_point.y + 1);
	printf("                       #                                        #   # ");
	CSgotoxy(start_point.x, start_point.y + 2);
	printf("  ###    ###   # ##   ####    ##    # ##   #   #   ###             #  ");
	CSgotoxy(start_point.x, start_point.y + 3);
	printf(" #   #  #   #  ##  #   #       #    ##  #  #   #  #   #           #   ");
	CSgotoxy(start_point.x, start_point.y + 4);
	printf(" #      #   #  #   #   #       #    #   #  #   #  #####           #   ");
	CSgotoxy(start_point.x, start_point.y + 5);
	printf(" #   #  #   #  #   #   #  #    #    #   #  #  ##  #                   ");
	CSgotoxy(start_point.x, start_point.y + 6);
	printf("  ###    ###   #   #    ##    ###   #   #   ## #   ###            #   ");
}

void print_number(int number , vector2 start_point)
{

	//10.x = 71 / 79
	//10.y = 15 ~ 22 
	// x = 75
	// y = 15 ~ 22

	switch (number)
	{
		case 0:
			CSgotoxy(start_point.x, start_point.y);
			printf(" #### ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf(" #### ");
			break;
		case 1:
			CSgotoxy(start_point.x, start_point.y);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf(" ###  ");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf("  ##  ");
			break;
		case 2:
			CSgotoxy(start_point.x, start_point.y);
			printf(" #### ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("   ## ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf(" ##   ");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("##	  ");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf("######");
			break;
		case 3:
			CSgotoxy(start_point.x, start_point.y );
			printf(" #### ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("  ### ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf(" #### ");
			break;
		case 4:
			CSgotoxy(start_point.x, start_point.y);
			printf("   ## ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("  ### ");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("  ### ");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf(" # ## ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf(" # ## ");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("######");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("   ## ");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf("  ####");
			break;
		case 5:
			CSgotoxy(start_point.x, start_point.y);
			printf("######");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("##    ");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("##    ");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("##### ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf(" #### ");
			break;
		case 6:
			CSgotoxy(start_point.x, start_point.y);
			printf("  ### ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf(" ##   ");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("##    ");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("##### ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf(" #### ");
			break;
		case 7:
			CSgotoxy(start_point.x, start_point.y);
			printf("######");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("   ## ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("   ## ");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("  ##  ");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf("  ##  ");
			break;
		case 8:
			CSgotoxy(start_point.x, start_point.y);
			printf(" #### ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf(" #### ");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf(" #### ");
			break;
		case 9:
			CSgotoxy(start_point.x, start_point.y);
			printf(" #### ");
			CSgotoxy(start_point.x, start_point.y + 1);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 2);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 3);
			printf("##  ##");
			CSgotoxy(start_point.x, start_point.y + 4);
			printf(" #####");
			CSgotoxy(start_point.x, start_point.y + 5);
			printf("    ##");
			CSgotoxy(start_point.x, start_point.y + 6);
			printf("   ## ");
			CSgotoxy(start_point.x, start_point.y + 7);
			printf(" ###  ");
			break;

	}

}



void death(void)
{
	vector2 temp = { 71,15 };

	system("cls");
	print_number(1, temp);
	temp.x = 79; temp.y = 15;
	print_number(0, temp);
	temp.x = 49; temp.y = 3;
	print_continue(temp);
	Sleep(1000);

	
	for (int i = 9; i >= 0; i--)
	{
		system("cls");
		temp.x = 75; temp.y = 15;
		print_number(i, temp);
		temp.x = 49; temp.y = 3;
		print_continue(temp);
		Sleep(1000);
	}

	CSgotoxy(0, 30);
	exit(0);
}

void move_head(void)
{
	switch (way)
	{
	case UP:
		main_snake[0].y--;
		break;

	case DOWN:
		main_snake[0].y++;
		break;

	case LEFT:
		main_snake[0].x -= 2;
		break;

	case RIGHT:
		main_snake[0].x += 2;
		break;
	}


	if (main_snake[0].x == 0 || main_snake[0].x == 98 || main_snake[0].y == 0 || main_snake[0].y == 29)
	{
		death();
	}


	for (int i = 1; i < tail; i++)
	{
		if (main_snake[0].x == main_snake[i].x && main_snake[0].y == main_snake[i].y)
		{
			death();
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (main_snake[0].x == item[i].x && main_snake[0].y == item[i].y)
		{
			main_snake[tail + 1].x = main_snake[tail].x;
			main_snake[tail + 1].y = main_snake[tail].y;

			tail++;

			printf("\a");

			score += (int)game_time / 10 * 100 + 100;

			star_random(i);

		}

	}
}

void move_tail(void)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15<<4);
	CSgotoxy(main_snake[tail].x, main_snake[tail].y);
	printf("  ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);


	for (int i = tail; i > 0; i--)
	{
		main_snake[i].x = main_snake[i - 1].x;
		main_snake[i].y = main_snake[i - 1].y;
	}
}



void print_snake(void)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);

	CSgotoxy(main_snake[0].x, main_snake[0].y);
	//print_head();

	for (int i = 1; i <= tail; i++)
	{
		CSgotoxy(main_snake[i].x, main_snake[i].y);
		//print_body();
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
}

void delay(int x)
{
	Sleep(x);
}


void change_speed()
{
	if ((int)game_time % 10 == 0)
	{
		if (speed_delay == 0)
		{
			if (speed >= 10)
			{
				speed -= 10;
				speed_point += 2;
				speed_delay = 1;
			}
		}
	}
	else
	{
		speed_delay = 0;
	}
}


void print_score(void)
{

	CSgotoxy(105, 10);
	printf("┌──────────ScoreBoard──────────┐");
	CSgotoxy(105, 29);
	printf("└─────────────────────────┘");
	for (int i = 11; i < 29; i++)
	{
		CSgotoxy(105, i);
		printf("│                                                  │");
	}
	CSgotoxy(105, 17);
	printf("├─────────────────────────┤");
	CSgotoxy(109, 13);
	printf("TIME  : ");
	CSgotoxy(109, 15);
	printf("SCORE : ");
	CSgotoxy(109, 19);
	printf("SPEED : ");


}

void change_score(void)
{
	CSgotoxy(116, 13);
	printf("%f", game_time);
	CSgotoxy(116, 15);
	printf("%d", score);
	CSgotoxy(116, 19);
	printf("%d", (100 - speed) + 20);
	for (int i = 0; i <= speed_point; i += 2)
	{
		CSgotoxy(i + 109, 21);
		printf("◆");
	}
}

void print_coundsaddasdsad(void)
{
	/*
	for (int i = 10; i >= 0; i--)
	{
		system("cls");
		print_number(i);
		print_continue();
		Sleep(1000);
	}
	*/
}

int main(void)
{
	vector2 temp = {75, 15};

	initial_setting();

	main_menu :

	print_starting();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);

	switch (menu)
	{
	case START:
		system("cls");
		for (int i = 3; i > 0; i++)
		{
			print_number(i, temp);
			Sleep(1000);
		}
		printf("\a");

		system("cls");


		print_wall();

		temp.x = 108; temp.y = 2;
		print_title(temp);

		print_score();

		print_snake();

		star_random(0);
		star_random(1);

	
		while (1)
		{
			move_tail();

			move_head();

			print_snake();

			delay(speed);

			change_score();
			change_speed();
			
		}

		break;

	case SETTING:




		break;

	case EXIT:

		if (IDYES == MessageBox(NULL, TEXT("게임을 종료하시겠습니까?"), TEXT("EXIT"), MB_YESNO))
		{
			CSgotoxy(0,37);
			exit(0);
		}
		else
		{
			system("cls");

			goto main_menu;


		}

		exit(0);
	}

	

}


/*
/*
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <math.h>
#include <time.h>

typedef struct snake
{
int x;
int y;

}snake_struct;

snake_struct body[150] = { { 50,15 },{ 50,14 },{ 50,23 } };
snake_struct item = { 0,0 };
enum { UP, DOWN, LEFT, RIGHT };
int way = 0, score = 0, snake_tail = 2, speed = 100, sp = 1, speed_g = 1;
double result = 0;
int IsKeyDown(int Key);
void CSgotoxy(int X, int Y);

void  draw_background(); // 배경그리기.
void changeItem_pos(); // 아이템 위치갱신.
void get_tail(); // 꼬리 위치갱신.
void moveSnake(); // 뱀 이동.
void draw_snake_item(); // 뱀,아이템 출력
void draw_scoreboard();
void change_speed();
void change_score();
void draw_title();
void game_end();

UINT __stdcall Thread(void *arg)
{
while (1)
{
clock_t before;
before = clock();
if (way == UP || way == DOWN)
{
if (IsKeyDown(VK_LEFT))
way = LEFT;
if (IsKeyDown(VK_RIGHT))
way = RIGHT;
}
else if (way == RIGHT || way == LEFT)
{
if (IsKeyDown(VK_UP))
way = UP;
if (IsKeyDown(VK_DOWN))
way = DOWN;
}
result += (double)(clock() - before) / CLOCKS_PER_SEC;
}
}

int main(void)
{

draw_background();
Sleep(500);
draw_scoreboard();
Sleep(500);
changeItem_pos();
Sleep(500);
draw_title();
_beginthreadex(NULL, 0, Thread, 0, 0, NULL);

while (1)
{
change_speed();
change_score();
get_tail();
moveSnake();

if (body[0].x == 0 || body[0].x == 98 || body[0].y == 0 || body[0].y == 29) // 벽에 부딫혓을때.
game_end();

for (int i = 1; i < snake_tail; i++) // 몸과몸
if (body[0].x == body[i].x && body[0].y == body[i].y)
game_end();

if (body[0].x == item.x && body[0].y == item.y) // 아이템 먹엇을때.
{
body[snake_tail + 1].x = body[snake_tail].x;
body[snake_tail + 1].y = body[snake_tail].y;
score += (int)result / 10 * 100 + 100;
changeItem_pos();
}

draw_snake_item();
Sleep(speed);
}



return 0;
}

void  draw_background()
{
system("color 0F");
system("mode con: cols=163 lines=37");
system("title 뱀 게임^^^^7");
_beginthreadex(NULL, 0, Thread, 0, 0, NULL);



CONSOLE_CURSOR_INFO CurInfo;


CurInfo.dwSize = 1;
CurInfo.bVisible = FALSE;

SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);


SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);
CSgotoxy(0, 0);
printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
for (int i = 1; i < 29; i++)
{
CSgotoxy(0, i);
printf("■                                                                                                ■");
}
CSgotoxy(0, 29);
printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

}

void draw_scoreboard()
{
CSgotoxy(105, 10);
printf("┌──────────ScoreBoard──────────┐");
CSgotoxy(105, 29);
printf("└─────────────────────────┘");
for (int i = 11; i < 29; i++)
{
CSgotoxy(105, i);
printf("│                                                  │");
}
CSgotoxy(105, 17);
printf("├─────────────────────────┤");
CSgotoxy(109, 13);
printf("TIME  : ");
CSgotoxy(109, 15);
printf("SCORE : ");
CSgotoxy(109, 19);
printf("SPEED : ");
}

void change_score()
{
CSgotoxy(116, 13);
printf("%f", result);
CSgotoxy(116, 15);
printf("%d", score);
CSgotoxy(116, 19);
printf("%d", (100 - speed) + 20);
for (int i = 0; i <= speed_g; i += 2)
{
CSgotoxy(i + 109, 21);
printf("◆");
}
}

void draw_title()
{
CSgotoxy(108, 2);
printf(" ######   ##    ##     ###     ##    ##  ########  ");
CSgotoxy(108, 3);
printf("##    ##  ###   ##    ## ##    ##   ##   ##        ");
CSgotoxy(108, 4);
printf("##        ####  ##   ##   ##   ##  ##    ##        ");
CSgotoxy(108, 5);
printf(" ######   ## ## ##  ##     ##  #####     ######    ");
CSgotoxy(108, 6);
printf("      ##  ##  ####  #########  ##  ##    ##        ");
CSgotoxy(108, 7);
printf("##    ##  ##   ###  ##     ##  ##   ##   ##        ");
CSgotoxy(108, 8);
printf(" ######   ##    ##  ##     ##  ##    ##  ########  ");
}
void changeItem_pos()
{

item.x = ((rand() % 48) + 1) * 2;
item.y = ((rand() % 12) + 1) * 2;
}

int IsKeyDown(int Key)
{
return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

void CSgotoxy(int X, int Y)
{
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

COORD initial = { X,Y };

SetConsoleCursorPosition(hOut, initial);

}

void get_tail()
{
for (snake_tail = 0; snake_tail < 150; snake_tail++)
{
if (body[snake_tail].x == '\0')
{
snake_tail--;
break;
}
}
}

void draw_snake_item()
{
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);

CSgotoxy(body[0].x, body[0].y);
printf("●");

for (int i = 1; i <= snake_tail; i++)
{
CSgotoxy(body[i].x, body[i].y);
printf("○");
}
CSgotoxy(item.x, item.y);
printf("★");

SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void moveSnake()
{
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);

CSgotoxy(body[snake_tail].x, body[snake_tail].y);
printf(" ");

for (int i = snake_tail; i > 0; i--)
{
body[i].x = body[i - 1].x;
body[i].y = body[i - 1].y;
}

switch (way)
{
case UP:
body[0].y--;
break;

case DOWN:
body[0].y++;
break;

case LEFT:
body[0].x -= 2;
break;

case RIGHT:
body[0].x += 2;
break;
}

SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void change_speed()
{
if ((int)result % 10 == 0)
{
if (sp == 0)
{
if (speed >= 10)
{
speed -= 10;
speed_g += 2;
sp = 1;
}
}
}
else
{
sp = 0;
}
}

void game_end()
{

CSgotoxy(1, 31);
exit(0);
}

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <math.h>
#include <time.h>
#include <locale.h>

int IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

typedef struct snake
{
	int x;
	int y;
}snake;
enum { START, SETTING, EXIT };
enum { UP, DOWN, LEFT, RIGHT };
enum { DELAY, STAR };
//enum { ●, △, ▲ };  print_head
//   0   1   2
//enum { ★, ◆, ♠, ♥, ♣ }; print_item
//   0   1   2   3   4
//enum { ○, ●, ◎, ⊙, ▣, ◇, ◆, ◈, ♡, ♥, ♧, ♣, ♤, ♠}; print_body
//   0   1   2   3   4   5   6   7   8   9   10  11  12  13
int speed = 100;
int speed_delay = 1;
int speed_point = 1;
double game_time = 0;
int menu = 0;
int way = DOWN;
int head_check = 0;
int item_check = 0;
int body_check = 0;
snake main_snake[150] = { { 50,15 },{ 50,14 },{ 50,13 } };
snake item[2] = { { 0,0 },{ 0,0 } };
int tail = 2;
int score = 0;


void CSgotoxy(int X, int Y)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD initial = { X,Y };
	SetConsoleCursorPosition(hOut, initial);
}

UINT __stdcall Thread(void *arg)
{
	while (1)
	{
		clock_t before;
		before = clock();
		if (way == UP || way == DOWN)
		{
			if (IsKeyDown(VK_LEFT))
				way = LEFT;
			if (IsKeyDown(VK_RIGHT))
				way = RIGHT;
		}
		else if (way == RIGHT || way == LEFT)
		{
			if (IsKeyDown(VK_UP))
				way = UP;
			if (IsKeyDown(VK_DOWN))
				way = DOWN;
		}
		game_time += (double)(clock() - before) / CLOCKS_PER_SEC;

	}
}

void print_item(void)
{
	//enum { ●, △, ▲ };  print_head
	//   0   1   2
	//enum { ★, ◆, ♠, ♥, ♣ }; print_item
	//   0   1   2   3   4
	//enum { ○, ●, ◎, ⊙, ▣, ◇, ◆, ◈, ♡, ♥, ♧, ♣, ♤, ♠}; print_body
	//   0   1   2   3   4   5   6   7   8   9   10  11  12  13
	switch (item_check)
	{
	case 0:
		printf("★");
		break;
	case 1:
		printf("◆");
		break;
	case 2:
		printf("♠");
		break;
	case 3:
		printf("♥");
		break;
	case 4:
		printf("♣");
		break;
	}
}

void star_random(int x)
{
	srand((unsigned int)time(NULL));

re_rand:
	item[x].x = ((rand() % 47) + 1) * 2;
	item[x].y = ((rand() % 14) + 1) * 2;
	if (x == 0)
	{
		if ((item[x].x == item[1].x) && (item[x].y == item[1].y))
		{
			goto re_rand;
		}
	}
	else
	{
		if ((item[x].x == item[0].x) && (item[x].y == item[0].y))
		{
			goto re_rand;
		}
	}

	for (int i = 0; i < tail; i++)
	{
		if ((main_snake[i].x == item[x].x) && (main_snake[i].y == item[x].y))
		{
			goto re_rand;
		}
	}
	CSgotoxy(item[x].x, item[x].y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);
	print_item();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

}

void initial_setting(void)
{
	system("color 0F");
	system("mode con: cols=163 lines=37");
	system("title 뱀 게임^^^^7");
	_beginthreadex(NULL, 0, Thread, 0, 0, NULL);

	CONSOLE_CURSOR_INFO CurInfo;

	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);


}

void print_starting(void)
{
	DWORD  dw, dwmode;
	INPUT_RECORD rec;

	printf("                                     ___                ____               ___                  ___                    ____                  \n");
	printf("                                    /  /＼             /  / ＼            /  /＼               /  /＼                 /  / ＼                \n");
	printf("                                   /  /: :＼          /  /: :|           /  /: :＼            /  / :/                /  / : :＼              \n");
	printf("                                  /__/:/＼: ＼       /  /: |:|          /  /: /＼:＼         /  / :/                /  / : /＼:＼            \n");
	printf("                               ___＼_＼:＼ ＼:＼    /  /: /|:|__       /  /:: ＼ ＼:＼      /  / ::＼________      /  / :  ＼ ＼:＼          \n");
	printf("                              /___/＼  ＼:＼ ＼:＼ /__/ :/ |:| /＼    /__/: /＼:＼_＼:＼   /  / :/＼    :::::＼   /__/ : /＼:＼ ＼:＼        \n");
	printf("                              ＼  ＼:＼  ＼:＼_＼/ ＼_＼/  |:|/ :/    ＼_＼/   ＼:＼/: /  /__/ :/~~| :|~~~~~~~~   ＼  ＼:＼ ＼:＼_＼/        \n");
	printf("                                ＼  ＼:＼__＼:＼       |   |:/ :/           ＼___＼:: /   ＼_＼/   | :|             ＼  ＼:＼ ＼:＼          \n");
	printf("                                  ＼  ＼:＼ /:/        |___|: :/             /   / : /         |   | :|               ＼  ＼:＼_＼/          \n");
	printf("                                    ＼  ＼ ::/         /__/ : /             /___/  :/          |___| :|                 ＼  ＼:＼            \n");
	printf("                                      ＼__＼/          ＼_＼_/              ＼___＼/            ＼__＼|                   ＼__＼/            \n");

	CSgotoxy(64, 17);
	printf("┌──────────────┐");
	CSgotoxy(64, 18);
	printf("│          게임시작          │");
	CSgotoxy(64, 19);
	printf("└──────────────┘");
	CSgotoxy(64, 21);
	printf("┌──────────────┐");
	CSgotoxy(64, 22);
	printf("│          환경설정          │");
	CSgotoxy(64, 23);
	printf("└──────────────┘");

	CSgotoxy(64, 25);
	printf("┌──────────────┐");
	CSgotoxy(64, 26);
	printf("│          게임종료          │");
	CSgotoxy(64, 27);
	printf("└──────────────┘");

	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwmode) | ENABLE_MOUSE_INPUT);
	while (1)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dw);
		if (rec.EventType == MOUSE_EVENT)
		{
			if ((rec.Event.MouseEvent.dwMousePosition.X > 64 && rec.Event.MouseEvent.dwMousePosition.X < 95) && (rec.Event.MouseEvent.dwMousePosition.Y > 16 && rec.Event.MouseEvent.dwMousePosition.Y < 20))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				CSgotoxy(64, 17);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 18);
				printf("│          게임시작          │  ");
				CSgotoxy(64, 19);
				printf("└──────────────┘  ");
				CSgotoxy(0, 30);
				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					menu = START;
					return;
				}
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				CSgotoxy(64, 17);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 18);
				printf("│          게임시작          │  ");
				CSgotoxy(64, 19);
				printf("└──────────────┘  ");
				CSgotoxy(0, 30);
			}

			if ((rec.Event.MouseEvent.dwMousePosition.X > 64 && rec.Event.MouseEvent.dwMousePosition.X < 95) && (rec.Event.MouseEvent.dwMousePosition.Y > 20 && rec.Event.MouseEvent.dwMousePosition.Y < 24))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				CSgotoxy(64, 21);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 22);
				printf("│          환경설정          │  ");
				CSgotoxy(64, 23);
				printf("└──────────────┘  ");
				CSgotoxy(0, 30);
				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					menu = SETTING;
					return;
				}


			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				CSgotoxy(64, 21);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 22);
				printf("│          환경설정          │  ");
				CSgotoxy(64, 23);
				printf("└──────────────┘  ");
				CSgotoxy(0, 30);
			}


			if ((rec.Event.MouseEvent.dwMousePosition.X > 64 && rec.Event.MouseEvent.dwMousePosition.X < 95) && (rec.Event.MouseEvent.dwMousePosition.Y > 24 && rec.Event.MouseEvent.dwMousePosition.Y < 28))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				CSgotoxy(64, 25);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 26);
				printf("│          게임종료          │  ");
				CSgotoxy(64, 27);
				printf("└──────────────┘  ");
				CSgotoxy(0, 30);
				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					menu = EXIT;
					return;
				}

			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				CSgotoxy(64, 25);
				printf("┌──────────────┐  ");
				CSgotoxy(64, 26);
				printf("│          게임종료          │  ");
				CSgotoxy(64, 27);
				printf("└──────────────┘  ");
				CSgotoxy(0, 30);
			}

		}

	}

}

void print_wall(void)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);
	CSgotoxy(0, 0);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	for (int i = 1; i < 29; i++)
	{
		CSgotoxy(0, i);
		printf("■                                                                                                ■");
	}
	CSgotoxy(0, 29);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void print_title(void)
{
	CSgotoxy(108, 2);
	printf(" ######   ##    ##     ###     ##    ##  ########  ");
	CSgotoxy(108, 3);
	printf("##    ##  ###   ##    ## ##    ##   ##   ##        ");
	CSgotoxy(108, 4);
	printf("##        ####  ##   ##   ##   ##  ##    ##        ");
	CSgotoxy(108, 5);
	printf(" ######   ## ## ##  ##     ##  #####     ######    ");
	CSgotoxy(108, 6);
	printf("      ##  ##  ####  #########  ##  ##    ##        ");
	CSgotoxy(108, 7);
	printf("##    ##  ##   ###  ##     ##  ##   ##   ##        ");
	CSgotoxy(108, 8);
	printf(" ######   ##    ##  ##     ##  ##    ##  ########  ");
}

void print_head(void)
{
	//enum { ●, △, ▲ };  print_head
	//   0   1   2
	//enum { ★, ◆, ♠, ♥, ♣ }; print_item
	//   0   1   2   3   4
	//enum { ○, ●, ◎, ⊙, ▣, ◇, ◆, ◈, ♡, ♥, ♧, ♣, ♤, ♠}; print_body
	//   0   1   2   3   4   5   6   7   8   9   10  11  12  13

	switch (head_check)
	{
	case 0:
		printf("●");
		break;
	case 1:
		printf("△");
		break;
	case 2:
		printf("▲");
		break;
	}

}
void print_body(void)
{
	//enum { ●, △, ▲ };  print_head
	//   0   1   2
	//enum { ★, ◆, ♠, ♥, ♣ }; print_item
	//   0   1   2   3   4
	//enum { ○, ●, ◎, ⊙, ▣, ◇, ◆, ◈, ♡, ♥, ♧, ♣, ♤, ♠}; print_body
	//   0   1   2   3   4   5   6   7   8   9   10  11  12  13

	switch (body_check)
	{
	case 0:
		printf("○");
		break;
	case 1:
		printf("●");
		break;
	case 2:
		printf("◎");
		break;
	case 3:
		printf("⊙");
		break;
	case 4:
		printf("▣");
		break;
	case 5:
		printf("◇");
		break;
	case 6:
		printf("◆");
		break;
	case 7:
		printf("◈");
		break;
	case 8:
		printf("♡");
		break;
	case 9:
		printf("♥");
		break;
	case 10:
		printf("♧");
		break;
	case 11:
		printf("♣");
		break;
	case 12:
		printf("♤");
		break;
	case 13:
		printf("♠");
		break;

	}
}
void print_snake(void)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);
	CSgotoxy(main_snake[0].x, main_snake[0].y);
	print_head();
	for (int i = 1; i <= tail; i++)
	{
		CSgotoxy(main_snake[i].x, main_snake[i].y);
		print_body();
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void print_continue(void)
{
	CSgotoxy(49, 3);
	printf("                       #       #                                 ###  ");
	CSgotoxy(49, 4);
	printf("                       #                                        #   # ");
	CSgotoxy(49, 5);
	printf("  ###    ###   # ##   ####    ##    # ##   #   #   ###             #  ");
	CSgotoxy(49, 6);
	printf(" #   #  #   #  ##  #   #       #    ##  #  #   #  #   #           #   ");
	CSgotoxy(49, 7);
	printf(" #      #   #  #   #   #       #    #   #  #   #  #####           #   ");
	CSgotoxy(49, 8);
	printf(" #   #  #   #  #   #   #  #    #    #   #  #  ##  #                   ");
	CSgotoxy(49, 9);
	printf("  ###    ###   #   #    ##    ###   #   #   ## #   ###            #   ");
}
void print_number(int number)
{
	switch (number)
	{
	case 0:
		CSgotoxy(75, 15);
		printf(" #### ");
		CSgotoxy(75, 16);
		printf("##  ##");
		CSgotoxy(75, 17);
		printf("##  ##");
		CSgotoxy(75, 18);
		printf("##  ##");
		CSgotoxy(75, 19);
		printf("##  ##");
		CSgotoxy(75, 20);
		printf("##  ##");
		CSgotoxy(75, 21);
		printf("##  ##");
		CSgotoxy(75, 22);
		printf(" #### ");
		break;
	case 1:
		CSgotoxy(75, 15);
		printf("  ##  ");
		CSgotoxy(75, 16);
		printf(" ###  ");
		CSgotoxy(75, 17);
		printf("  ##  ");
		CSgotoxy(75, 18);
		printf("  ##  ");
		CSgotoxy(75, 19);
		printf("  ##  ");
		CSgotoxy(75, 20);
		printf("  ##  ");
		CSgotoxy(75, 21);
		printf("  ##  ");
		CSgotoxy(75, 22);
		printf("  ##  ");
		break;
	case 2:
		system("cls");
		CSgotoxy(75, 15);
		printf(" #### ");
		CSgotoxy(75, 16);
		printf("##  ##");
		CSgotoxy(75, 17);
		printf("    ##");
		CSgotoxy(75, 18);
		printf("   ## ");
		CSgotoxy(75, 19);
		printf("  ##  ");
		CSgotoxy(75, 20);
		printf(" ##   ");
		CSgotoxy(75, 21);
		printf("##   ");
		CSgotoxy(75, 22);
		printf("######");
		break;
	case 3:
		CSgotoxy(75, 15);
		printf(" #### ");
		CSgotoxy(75, 16);
		printf("##  ##");
		CSgotoxy(75, 17);
		printf("    ##");
		CSgotoxy(75, 18);
		printf("  ### ");
		CSgotoxy(75, 19);
		printf("    ##");
		CSgotoxy(75, 20);
		printf("    ##");
		CSgotoxy(75, 21);
		printf("##  ##");
		CSgotoxy(75, 22);
		printf(" #### ");
		break;
	case 4:
		CSgotoxy(75, 15);
		printf("   ## ");
		CSgotoxy(75, 16);
		printf("  ### ");
		CSgotoxy(75, 17);
		printf("  ### ");
		CSgotoxy(75, 18);
		printf(" # ## ");
		CSgotoxy(75, 19);
		printf(" # ## ");
		CSgotoxy(75, 20);
		printf("######");
		CSgotoxy(75, 21);
		printf("   ## ");
		CSgotoxy(75, 22);
		printf("  ####");
		break;
	case 5:
		CSgotoxy(75, 15);
		printf("######");
		CSgotoxy(75, 16);
		printf("##    ");
		CSgotoxy(75, 17);
		printf("##    ");
		CSgotoxy(75, 18);
		printf("##### ");
		CSgotoxy(75, 19);
		printf("    ##");
		CSgotoxy(75, 20);
		printf("    ##");
		CSgotoxy(75, 21);
		printf("##  ##");
		CSgotoxy(75, 22);
		printf(" #### ");
		break;
	case 6:
		CSgotoxy(75, 15);
		printf("  ### ");
		CSgotoxy(75, 16);
		printf(" ##   ");
		CSgotoxy(75, 17);
		printf("##    ");
		CSgotoxy(75, 18);
		printf("##### ");
		CSgotoxy(75, 19);
		printf("##  ##");
		CSgotoxy(75, 20);
		printf("##  ##");
		CSgotoxy(75, 21);
		printf("##  ##");
		CSgotoxy(75, 22);
		printf(" #### ");
		break;
	case 7:
		CSgotoxy(75, 15);
		printf("######");
		CSgotoxy(75, 16);
		printf("##  ##");
		CSgotoxy(75, 17);
		printf("    ##");
		CSgotoxy(75, 18);
		printf("   ## ");
		CSgotoxy(75, 19);
		printf("   ## ");
		CSgotoxy(75, 20);
		printf("  ##  ");
		CSgotoxy(75, 21);
		printf("  ##  ");
		CSgotoxy(75, 22);
		printf("  ##  ");
		break;
	case 8:
		CSgotoxy(75, 15);
		printf(" #### ");
		CSgotoxy(75, 16);
		printf("##  ##");
		CSgotoxy(75, 17);
		printf("##  ##");
		CSgotoxy(75, 18);
		printf(" #### ");
		CSgotoxy(75, 19);
		printf("##  ##");
		CSgotoxy(75, 20);
		printf("##  ##");
		CSgotoxy(75, 21);
		printf("##  ##");
		CSgotoxy(75, 22);
		printf(" #### ");
		break;
	case 9:
		CSgotoxy(75, 15);
		printf(" #### ");
		CSgotoxy(75, 16);
		printf("##  ##");
		CSgotoxy(75, 17);
		printf("##  ##");
		CSgotoxy(75, 18);
		printf("##  ##");
		CSgotoxy(75, 19);
		printf(" #####");
		CSgotoxy(75, 20);
		printf("    ##");
		CSgotoxy(75, 21);
		printf("   ## ");
		CSgotoxy(75, 22);
		printf(" ###  ");
		break;
	case 10:
		CSgotoxy(71, 15);
		printf("  ##  ");
		CSgotoxy(71, 16);
		printf(" ###  ");
		CSgotoxy(71, 17);
		printf("  ##  ");
		CSgotoxy(71, 18);
		printf("  ##  ");
		CSgotoxy(71, 19);
		printf("  ##  ");
		CSgotoxy(71, 20);
		printf("  ##  ");
		CSgotoxy(71, 21);
		printf("  ##  ");
		CSgotoxy(71, 22);
		printf("  ##  ");

		CSgotoxy(79, 15);
		printf(" #### ");
		CSgotoxy(79, 16);
		printf("##  ##");
		CSgotoxy(79, 17);
		printf("##  ##");
		CSgotoxy(79, 18);
		printf("##  ##");
		CSgotoxy(79, 19);
		printf("##  ##");
		CSgotoxy(79, 20);
		printf("##  ##");
		CSgotoxy(79, 21);
		printf("##  ##");
		CSgotoxy(79, 22);
		printf(" #### ");
		break;

	}
}

void death(void)
{
	for (int i = 10; i >= 0; i--)
	{
		system("cls");
		print_number(i);
		print_continue();
		Sleep(1000);
	}
	CSgotoxy(0, 30);
	exit(0);
}
void move_head(void)
{
	switch (way)
	{
	case UP:
		main_snake[0].y--;
		break;
	case DOWN:
		main_snake[0].y++;
		break;
	case LEFT:
		main_snake[0].x -= 2;
		break;
	case RIGHT:
		main_snake[0].x += 2;
		break;
	}

	if (main_snake[0].x == 0 || main_snake[0].x == 98 || main_snake[0].y == 0 || main_snake[0].y == 29)
	{
		death();
	}

	for (int i = 1; i < tail; i++)
	{
		if (main_snake[0].x == main_snake[i].x && main_snake[0].y == main_snake[i].y)
		{
			death();
		}
	}
	for (int i = 0; i < 2; i++)
	{
		if (main_snake[0].x == item[i].x && main_snake[0].y == item[i].y)
		{
			main_snake[tail + 1].x = main_snake[tail].x;
			main_snake[tail + 1].y = main_snake[tail].y;
			tail++;
			printf("\a");
			score += (int)game_time / 10 * 100 + 100;
			star_random(i);
		}
	}
}
void move_tail(void)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15 << 4);
	CSgotoxy(main_snake[tail].x, main_snake[tail].y);
	printf("  ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

	for (int i = tail; i > 0; i--)
	{
		main_snake[i].x = main_snake[i - 1].x;
		main_snake[i].y = main_snake[i - 1].y;
	}
}
void delay(int x)
{
	Sleep(x);
}

void change_speed()
{
	if ((int)game_time % 10 == 0)
	{
		if (speed_delay == 0)
		{
			if (speed >= 10)
			{
				speed -= 10;
				speed_point += 2;
				speed_delay = 1;
			}
		}
	}
	else
	{
		speed_delay = 0;
	}
}

void print_score(void)
{
	CSgotoxy(105, 10);
	printf("┌──────────ScoreBoard──────────┐");
	CSgotoxy(105, 29);
	printf("└─────────────────────────┘");
	for (int i = 11; i < 29; i++)
	{
		CSgotoxy(105, i);
		printf("│                                                  │");
	}
	CSgotoxy(105, 17);
	printf("├─────────────────────────┤");
	CSgotoxy(109, 13);
	printf("TIME  : ");
	CSgotoxy(109, 15);
	printf("SCORE : ");
	CSgotoxy(109, 19);
	printf("SPEED : ");

}
void change_score(void)
{
	CSgotoxy(116, 13);
	printf("%f", game_time);
	CSgotoxy(116, 15);
	printf("%d", score);
	CSgotoxy(116, 19);
	printf("%d", (100 - speed) + 20);
	for (int i = 0; i <= speed_point; i += 2)
	{
		CSgotoxy(i + 109, 21);
		printf("◆");
	}
}
void print_count(void)
{
	for (int i = 10; i >= 0; i--)
	{
		system("cls");
		print_number(i);
		print_continue();
		Sleep(1000);
	}
}

int main(void)
{
	initial_setting();
main_menu:
	print_starting();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	switch (menu)
	{
	case START:
		system("cls");
		print_number(3);
		Sleep(1000);
		print_number(2);
		Sleep(1000);
		print_number(1);
		Sleep(1000);
		system("cls");
		printf("\a");
		print_wall();
		print_title();
		print_score();
		print_snake();
		star_random(0);
		star_random(1);

		while (1)
		{
			move_tail();
			move_head();
			print_snake();
			delay(speed);
			change_score();
			change_speed();

		}
		break;
	case SETTING:


		break;
	case EXIT:
		if (IDYES == MessageBox(NULL, TEXT("게임을 종료하시겠습니까?"), TEXT("EXIT"), MB_YESNO))
		{
			CSgotoxy(0, 37);
			exit(0);
		}
		else
		{
			system("cls");
			goto main_menu;

		}
		exit(0);
	}

}
*/