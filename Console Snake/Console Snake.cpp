#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <thread>

using namespace std;

#define WIDTH 20
#define HEIGHT 20

bool gameOver;
int x, y, fruitY, fruitX, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void HideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;	
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void FruitGenerator() {
	fruitY = rand() % HEIGHT;
	fruitX = rand() % WIDTH;
}

void Setup() {
	gameOver = false;
	x = WIDTH / 2;
	y = HEIGHT / 2;
	FruitGenerator();
	score = 0;
}

void Draw() {
	system("cls"); // clear map
	HideCursor();
	for (int i = 0; i < WIDTH + 2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (j == 0)
				cout << "#";
			if (i == y && j == x)
				cout << "O";
			else if (i == fruitY && j == fruitX)
				cout << "$";
			else
			{
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}


			if (j == WIDTH - 1)
				cout << "#";
		}
		cout << endl;
	}

	for (int i = 0; i < WIDTH + 2; i++)
		cout << "#";
	cout << endl;
	cout << "Score:" << score << endl;
}

void Input() {
	if(_kbhit()){
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}

void Logic() 
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (dir)
	{
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	default:
		break;
	}/*
	if (x > WIDTH || y > HEIGHT || x < 0 || y < 0) 
		gameOver = true;*/
	if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
	if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;

	if (x == fruitX && y == fruitY)
	{
		score += 10;
		FruitGenerator();
		nTail++;
	}
}

int main()
{
	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
		if (gameOver) break;
		this_thread::sleep_for(0.1s);
	}

	return 0;
}