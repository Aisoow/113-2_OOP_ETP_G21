#ifndef SNAKE_H
#define SNAKE_H

class Snake
{
private:
    int headX, headY;           // 蛇頭坐標
    int bodyX[100], bodyY[100]; // 蛇身坐標
    int length;                 // 蛇身長度

public:
    Snake();
    void setHead(int x, int y);            // 設定蛇頭坐標
    void setBody(int index, int x, int y); // 設定特定索引處的蛇身坐標
    void setLength(int len);               // 設定蛇身長度
    int getHeadX();                        // 取得蛇頭X坐標
    int getHeadY();                        // 取得蛇頭Y坐標
    int getBodyX(int index);               // 取得蛇身X坐標
    int getBodyY(int index);               // 取得蛇身Y坐標
    int getLength() const;                 // 取得蛇身長度
};
#endif