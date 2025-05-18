#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Snake.h"
#include "Food.h"

// 定義地圖元素類型
enum MapElement
{
    EMPTY = 0, // 空格
    WALL = 1,  // 牆
    BODY = 2,  // 蛇身
    HEAD = 3,  // 蛇頭
    FOOD = 4   // 食物
};

class Gamemap
{
private:
    int width;
    int height;
    int **map;            // 地圖二元陣列
    void allocateMap();   // 分配map的記憶體
    void deallocateMap(); // 釋放map的記憶體
public:
    Gamemap();
    Gamemap(int w, int h);                // 建構子
    ~Gamemap();                           // 解構子
    void setWidth(int w);                 // 設定地圖寬度
    void setHeight(int h);                // 設定地圖高度
    int getWidth();                       // 取得地圖寬度
    int getHeight();                      // 取得地圖高度
    void setMap(int x, int y, int value); // 設定地圖特定區域的值
    int getMap(int x, int y);             // 取得地圖特定區域的值
    void printMap();                      // 印出地圖
    void clearMap();                      // 清除地圖
    void initMap();                       // 初始化地圖
    void draw(Snake &snake, Food &food);  // 繪製整個遊戲畫面
};
#endif // !GAMEMAP_H