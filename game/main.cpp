#include "Snake.h"
#include <windows.h> //用于Sleep函数
#include <conio.h>

int main() {
	Snake snake;
	while (true) {
		if (_kbhit()) { // 检测键盘输入
			char key = _getch();// 获取按键
			snake.ChangeDirection(key);
		}

		snake.Move();
		snake.Draw();
		Sleep(100); // 控制游戏速度
	}
	return 0;
}