#include "Gamemap.h"
#include "Snake.h"
#include "Food.h"
#include <iostream>
#include <windows.h>
// 主體程式
using namespace std;

Gamemap::Gamemap() : width(0), height(0), map(nullptr) {}

Gamemap::~Gamemap()
{
    deallocateMap();
}

Gamemap::Gamemap(int w, int h) : width(w), height(h)
{
    allocateMap();
    initMap();
}

void Gamemap::allocateMap() // 分配map的記憶體
{
    if (width <= 0 || height <= 0) // 如果寬度或高度小於等於0，則不分配記憶體
    {
        throw invalid_argument("Width and height must be positive");
    }
    map = new int *[width]; // 分配一維陣列
    for (int i = 0; i < width; i++)
    {
        map[i] = new int[height]; // 分配二維陣列
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
    map = nullptr;
}

void Gamemap::setWidth(int w) // 設定地圖寬度
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

void Gamemap::setMap(int x, int y, int value) // 設定地圖特定區域的值
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        map[x][y] = value;
    }
}

int Gamemap::getMap(int x, int y) // 取得地圖特定區域的值
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return map[x][y];
    }
    return WALL; // 超出邊界返回牆
}

void Gamemap::printMap() // 印出地圖
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos;

    // 一次性設定控制台緩衝區，避免閃爍
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    SMALL_RECT windowRect = csbi.srWindow;
    SHORT width = windowRect.Right - windowRect.Left + 1;
    SHORT height = windowRect.Bottom - windowRect.Top + 1;

    // 優化繪製過程
    for (int i = 0; i < this->height; i++)
    {
        cursorPos.Y = i;
        SetConsoleCursorPosition(hConsole, cursorPos);

        for (int j = 0; j < this->width; j++)
        {
            cursorPos.X = j * 2; // 每個圖形佔用2個字元寬度
            SetConsoleCursorPosition(hConsole, cursorPos);

            switch (map[j][i])
            {
            case EMPTY:                               // 空格
                SetConsoleTextAttribute(hConsole, 0); // 黑色背景
                cout << "  ";
                break;
            case WALL:                                  // 牆
                SetConsoleTextAttribute(hConsole, 119); // 淺藍色背景，白色前景
                cout << "[]";
                break;
            case BODY:                                 // 蛇身
                SetConsoleTextAttribute(hConsole, 42); // 綠色背景，紅色前景
                cout << "◆";
                break;
            case HEAD:                                 // 蛇頭
                SetConsoleTextAttribute(hConsole, 46); // 黃色背景，紅色前景
                cout << "◎";
                break;
            case FOOD:                                  // 食物
                SetConsoleTextAttribute(hConsole, 252); // 紅色背景，白色前景
                cout << "★";
                break;
            default:
                SetConsoleTextAttribute(hConsole, 7); // 預設
                cout << "  ";
            }
        }
    }
    SetConsoleTextAttribute(hConsole, 7); // 恢復白色
}

void Gamemap::clearMap() // 清除地圖
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            map[j][i] = EMPTY; // 全部設為空格
        }
    }
}

void Gamemap::initMap() // 初始化地圖
{
    clearMap(); // 先清空地圖

    // 設定牆壁
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) // 邊界
            {
                map[j][i] = WALL; // 牆
            }
        }
    }
}

void Gamemap::draw(Snake &snake, Food &food)
{
    // 首先清空現有地圖（但保留牆壁）
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            map[j][i] = EMPTY;
        }
    }

    // 繪製食物
    int foodX = food.getX();
    int foodY = food.getY();
    if (foodX > 0 && foodX < width - 1 && foodY > 0 && foodY < height - 1)
    {
        map[foodX][foodY] = FOOD;
    }

    // 繪製蛇頭
    int headX = snake.getHeadX();
    int headY = snake.getHeadY();
    if (headX > 0 && headX < width - 1 && headY > 0 && headY < height - 1)
    {
        map[headX][headY] = HEAD;
    }

    // 繪製蛇身
    for (int i = 0; i < snake.getLength(); i++)
    {
        int bodyX = snake.getBodyX(i);
        int bodyY = snake.getBodyY(i);
        if (bodyX > 0 && bodyX < width - 1 && bodyY > 0 && bodyY < height - 1)
        {
            map[bodyX][bodyY] = BODY;
        }
    }

    // 印出地圖
    system("cls");
    printMap();
}