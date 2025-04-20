#include "Gamemap.h"
#include <iostream>
using namespace std;
Gamemap::Gamemap() {}
Gamemap::Gamemap(int w, int h) : width(w), height(h) {}
void Gamemap::allocateMap() // 分配map的記憶體
{
    if (width <= 0 || height <= 0) // 如果寬度或高度小於等於0，則不分配記憶體
    {
        throw invalid_argument("Width and height must be positive");
    }
    map = new bool *[width]; // 分配一維陣列
    for (int i = 0; i < width; i++)
    {
        map[i] = new bool[height]; // 分配二維陣列
    }
}

void Gamemap::deallocateMap() // 釋放map的記憶體
{
    if (map == nullptr)
        return; // 如果map已經是空指標，則不需要釋放
    for (int i = 0; i < width; i++)
    {
        delete[] map[i]; // 釋放二維陣列
    }
    delete[] map; // 釋放一維陣列
}
void Gamemap::setWidth(int w) // 設定地圖寬度
// 這裡的w是指寬度的長度 不是指邊界的長度
{
    if (w <= 0) // 如果寬度小於等於0，則不設定
    {
        throw invalid_argument("Width must be positive");
    }
    if (w != width)
    {
        deallocateMap();
        width = w;
        allocateMap();
        initMap();
    }
}
void Gamemap::setHeight(int h) // 設定地圖高度
// 這裡的h是指高度的長度 不是指邊界的長度
{
    if (h <= 0) // 如果高度小於等於0，則不設定
    {
        throw invalid_argument("Height must be positive");
    }
    if (h != height)
    {
        deallocateMap();
        height = h;
        allocateMap();
        initMap();
    }
}
int Gamemap::getWidth() // 取得地圖寬度
{
    return width;
}
int Gamemap::getHeight() // 取得地圖高度
{
    return height;
}
void Gamemap::setMap(int x, int y, bool value) // 設定地圖特定區域的值
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        map[x][y] = value;
    }
}
bool Gamemap::getMap(int x, int y) // 取得地圖特定區域的值
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return map[x][y];
    }
    return false; // 超出邊界返回false
}
// void Gamemap::setBorder(int x, int y);
// bool Gamemap::getBorder(int x, int y);
void Gamemap::printMap() // 印出地圖
// 這裡用空格和■來表示地圖的內容 空格表示可通行的區域 ■表示不可通行的區域
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cout << map[j][i] ? " " : "■"; //
        }
        cout << endl;
    }
}
// void Gamemap::printBorder();
void Gamemap::clearMap() // 清除地圖
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            map[j][i] = false;
        }
    }
}
// void Gamemap::clearBorder();
void Gamemap::initMap() // 初始化地圖
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) // 邊界
            {
                map[j][i] = false;
            }
            else // 內部
            {
                map[j][i] = true;
            }
        }
    }
}