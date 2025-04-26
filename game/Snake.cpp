#include "Snake.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

Snake::Snake()
{
	srand(static_cast<unsigned int>(time(0))); // 随机数种子
	headX = rand() % (GAME_WIDTH - 4) + 2;
	headY = rand() % (GAME_HEIGHT - 4) + 2;
	dir = STOP;
	headChar = '^'; // 蛇头字符
}

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// 设置颜色函数
void SetGreen() {
	SetConsoleTextAttribute(hConsole,10);
}

void SetColor() {
	SetConsoleTextAttribute(hConsole, 7);
}


void Snake::ChangeDirection(char key)
{
	switch (key)
	{
	case 'w':
		if (dir != DOWN) {
			dir = UP;
			headChar = '^'; // 蛇头字符
		}
		break;
	case 's':
		if (dir != UP) {
			dir = DOWN;
			headChar = 'v';
		}
		break;
	case 'a':
		if (dir != RIGHT) {
			dir = LEFT;
			headChar = '<';
		}
		break;
	case 'd':
		if (dir != LEFT) {
			dir = RIGHT;
			headChar = '>';
		}
		break;
	}
}

void Snake::Move()
{
	// 检查边界
	if (dir == STOP) return;// 如果蛇没有移动方向，则返回
	int prevX = headX;// 保存蛇头的前一个坐标
	int prevY = headY;

	// 更新蛇头坐标
	switch (dir) {
	case UP: headY--; break;
	case DOWN: headY++; break;
	case LEFT: headX--; break;
	case RIGHT: headX++; break;
	}
	// 更新蛇身坐标 （后续扩展蛇身）
	if (!bodyX.empty()) {
		bodyX.insert(bodyX.begin(), prevX);
		bodyY.insert(bodyY.begin(), prevY);
		bodyX.pop_back();
		bodyY.pop_back();
	}
}

void Snake::Draw()
{
	system("cls"); // 清屏

	//绘制边框
	for (int i = 0; i < GAME_WIDTH + 2; i++)
		cout << "#";
	cout << endl;

	for (int y = 0; y < GAME_HEIGHT; y++) {
		cout << "#"; //左边框
		for (int x = 0; x < GAME_WIDTH; x++) {
			if (x == headX && y == headY) {
				SetGreen();
				cout << headChar; // 绘制蛇头
				SetColor();
			}
			else if (std::find(bodyX.begin(), bodyX.end(), x) != bodyX.end() &&
				std::find(bodyY.begin(), bodyY.end(), y) != bodyY.end()) {
				SetGreen();
				cout << "o"; // 绘制蛇身
				SetColor();
			}
			else {
				cout << " "; // 绘制空格
			}
		}
		cout << "#"; //右边框
		cout << endl;
	}

	//绘制底边框
	for (int i = 0; i < GAME_WIDTH + 2; i++)
		cout << "#";
	cout << endl;
}