#ifndef SNAKE_H
#define SNAKE_H	
#include <vector>
#include <Windows.h>

const int GAME_WIDTH = 40;
const int GAME_HEIGHT = 20;

enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

class Snake {
private:
	int headX, headY;//蛇头坐标
	Direction dir;//蛇头方向
	char headChar;//方向字符成员
	std::vector<int>bodyX;//蛇身坐标
	std::vector<int>bodyY;

public:
	Snake();

	void ChangeDirection(char key);//改变方向
	void Move();//移动
	void Draw();//绘制

	int GetHeadX() { return headX; }//获取蛇头坐标
	int GetHeadY() { return headY; }
};
#endif