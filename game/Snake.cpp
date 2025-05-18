#include "Snake.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Snake::Snake()
{
    // 初始化蛇的位置
    headX = 0;
    headY = 0;
    length = 0;

    // 初始化蛇身陣列
    for (int i = 0; i < 100; i++)
    {
        bodyX[i] = 0;
        bodyY[i] = 0;
    }
}

void Snake::setHead(int x, int y)
{
    headX = x;
    headY = y;
}

void Snake::setBody(int index, int x, int y)
{
    if (index >= 0 && index < 100)
    {
        bodyX[index] = x;
        bodyY[index] = y;
    }
}

void Snake::setLength(int len)
{
    if (len >= 0 && len < 100)
    {
        length = len;
    }
}

int Snake::getHeadX()
{
    return headX;
}

int Snake::getHeadY()
{
    return headY;
}

int Snake::getBodyX(int index)
{
    if (index >= 0 && index < length)
    {
        return bodyX[index];
    }
    return -1; // 返回無效位置
}

int Snake::getBodyY(int index)
{
    if (index >= 0 && index < length)
    {
        return bodyY[index];
    }
    return -1; // 返回無效位置
}

int Snake::getLength() const
{
    return length;
}