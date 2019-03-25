#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <math.h>



#define star printf("#")
#define enter printf(" ")



typedef struct point
{
	int X, Y;
}point;



void CSgotoxy(int X, int Y)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD initial = { X,Y };

	SetConsoleCursorPosition(hOut, initial);
}



void print(point p1, point p2)
{

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);


	COORD csX;
	COORD csY;

	point ODX;
	point NWX;

	point *psX;
	point *unpsX;

	point *psY;
	point *unpsY;


	double tempX = (double)p1.X - (double)p2.X;
	double tempY = (double)p1.Y - (double)p2.Y;







	if (p1.X == p2.X)
	{

		if (p1.Y < p2.Y)
		{
			psY = &p1;
			unpsY = &p2;
		}
		else if (p2.Y < p1.Y)
		{
			psY = &p2;
			unpsY = &p1;
		}
		else return;

		csY.X = p1.X;
		for (csY.Y = psY->Y; csY.Y <= unpsY->Y; csY.Y++)
		{
			SetConsoleCursorPosition(hOut, csY);

			// 수정 요함
			if (csY.X == 50 || csY.Y == 50)
			{
				// 색깔 수정 요함
				SetConsoleTextAttribute(hOut, 8 << 4);
				star;
				SetConsoleTextAttribute(hOut, 15);
			}
			else star;
		}



	}
	else
	{


		if (p1.X < p2.X)
		{
			psX = &p1;
			unpsX = &p2;
		}
		else if (p2.X < p1.X)
		{
			psX = &p2;
			unpsX = &p1;
		}




		//x축을 기준으로 한칸씩 출력
		for (csX.X = psX->X; csX.X <= unpsX->X; csX.X++)
		{

			csX.Y = (int)floor((tempY*(csX.X - (double)p1.X) / tempX + (double)p1.Y) + 0.5);

			//y을 기준으로 한칸씩 이동후 x에 출력 
			if (csX.X != psX->X)
			{
				NWX.X = csX.X; NWX.Y = csX.Y;




				if (ODX.Y == NWX.Y)
				{



				}
				else
				{

					if (ODX.Y < NWX.Y)
					{
						psY = &ODX;
						unpsY = &NWX;
					}
					else if (NWX.Y < ODX.Y)
					{
						psY = &NWX;
						unpsY = &ODX;
					}


					for (csY.Y = psY->Y; csY.Y <= unpsY->Y; csY.Y++)
					{
						csY.X = (int)floor((tempX*(csY.Y - (double)p1.Y) / tempY + (double)p1.X) + 0.5);
						SetConsoleCursorPosition(hOut, csY);
						// 수정 요함
						if (csY.X == 50 || csY.Y == 50)
						{
							// 색깔 수정 요함
							SetConsoleTextAttribute(hOut, 8 << 4);
							star;
							SetConsoleTextAttribute(hOut, 15);
						}
						else star;
					}
				}
			}


			ODX.Y = csX.Y; ODX.X = csX.X;


			SetConsoleCursorPosition(hOut, csX);
			// 수정 요함
			if (csX.X == 50 || csX.Y == 50)
			{
				// 색깔 수정 요함
				SetConsoleTextAttribute(hOut, 8 << 4);
				star;
				SetConsoleTextAttribute(hOut, 15);
			}
			else star;
		}
	}




}



int main()
{
	// 수정 요함
	system("mode con: cols=102 lines=102");

	system("title 두점을 선택하시면 이어드려요^^^^");


	INPUT_RECORD rec;
	DWORD dw, dwmode;
	point imp1;
	point unimp1;
	HANDLE hIn, hOut;

	int InputL = 0;
	int InputR = 0;

	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);


	// 색깔 수정 요함
	SetConsoleTextAttribute(hOut, 15);


	CSgotoxy(0, 1);
	printf("두점을 클릭하십시오");







	for (int i = 0; i <= 100; i++)
	{
		SetConsoleTextAttribute(hOut, 8 << 4);
		CSgotoxy(i, 50);
		enter;
	}

	for (int i = 0; i <= 100; i++)
	{
		SetConsoleTextAttribute(hOut, 8 << 4);
		CSgotoxy(50, i);
		enter;
	}
	SetConsoleTextAttribute(hOut, 15);



	SetConsoleMode(hIn, GetConsoleMode(hIn, &dwmode) | ENABLE_MOUSE_INPUT);

	for (;;) {

		// 사용자로부터 입력을 받습니다.
		ReadConsoleInput(hIn, &rec, 1, &dw);
		// 사용자로부터 받은 입력이 마우스일 경우,
		if (rec.EventType == MOUSE_EVENT)
			// 왼쪽 버튼이 눌렸을 경우,
			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED && InputL == 0)
			{
				// 커서의 위치를 왼쪽 버튼이 눌려진 위치로 이동합니다.
				CSgotoxy(rec.Event.MouseEvent.dwMousePosition.X, rec.Event.MouseEvent.dwMousePosition.Y);

				imp1.X = rec.Event.MouseEvent.dwMousePosition.X;
				imp1.Y = rec.Event.MouseEvent.dwMousePosition.Y;

				SetConsoleTextAttribute(hOut, 12);

				if (imp1.X == 50 || imp1.Y == 50)
				{
					SetConsoleTextAttribute(hOut, 8 << 4);
					star;
					SetConsoleTextAttribute(hOut, 15);
				}
				else star;

				SetConsoleTextAttribute(hOut, 15);

				CSgotoxy(0, 0);

				printf("%d, %d", imp1.X, imp1.Y);


				InputL = 1;
			}
		if (rec.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED && InputL == 1 && InputR == 0)
		{
			// 커서의 위치를 왼쪽 버튼이 눌려진 위치로 이동합니다.
			CSgotoxy(rec.Event.MouseEvent.dwMousePosition.X, rec.Event.MouseEvent.dwMousePosition.Y);
			// 해당 위치에 점을 찍습니다.

			unimp1.X = rec.Event.MouseEvent.dwMousePosition.X;
			unimp1.Y = rec.Event.MouseEvent.dwMousePosition.Y;

			SetConsoleTextAttribute(hOut, 12);

			if (unimp1.X == 50 || unimp1.Y == 50)
			{
				SetConsoleTextAttribute(hOut, 8 << 4);
				star;
				SetConsoleTextAttribute(hOut, 15);
			}
			else star;

			SetConsoleTextAttribute(hOut, 15);



			CSgotoxy(51, 0);


			printf("%d, %d", unimp1.X, unimp1.Y);


			InputR = 1;
		}
		if (rec.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK)
			break;
	}







	print(imp1, unimp1);




	CSgotoxy(imp1.X, imp1.Y);
	SetConsoleTextAttribute(hOut, 12);
	star;


	CSgotoxy(unimp1.X, unimp1.Y);
	star;




	SetConsoleTextAttribute(hOut, 15);

	CSgotoxy(0, 101);
	system("pause");


	return 0;

}