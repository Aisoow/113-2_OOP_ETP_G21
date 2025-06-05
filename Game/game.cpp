#include "Snake.h"
#include "Gamemap.h"
#include "Food.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>   // 加入檔案輸入輸出功能
#include <vector>    // 用於儲存多筆紀錄
#include <algorithm> // 用於排序紀錄
using namespace std;

// 定義方向常數
enum Direction
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// 定義紀錄結構
struct ScoreRecord
{
    string playerName;
    int score;
    string date;

    // 用於排序的比較運算子
    bool operator<(const ScoreRecord &other) const
    {
        return score > other.score; // 降序排列（高分在前）
    }
};

// 設定文字顏色
void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 將游標移動到指定位置
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 在控制台特定位置輸出文字
void printAt(int x, int y, const string &text)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << text;
}

// 徹底清除螢幕函數
void clearScreen()
{
    system("cls");
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            gotoxy(j, i);
            cout << " ";
        }
    }
}

// 建立預設紀錄，如果紀錄檔不存在
void createDefaultRecords()
{
    // 檢查紀錄檔是否存在
    ifstream checkFile("scores.txt");
    if (checkFile.is_open())
    {
        // 檔案存在，不需要建立
        checkFile.close();
        return;
    }

    // 檔案不存在，建立預設紀錄
    ofstream outFile("scores.txt");
    if (outFile.is_open())
    {
        // 獲取當前時間作為示範紀錄的時間
        time_t now = time(0);
        string dt = ctime(&now);
        dt = dt.substr(0, dt.length() - 1); // 去除換行符

        // 寫入三筆示範紀錄
        outFile << "王小明,50," << dt << endl;
        outFile << "李大華,30," << dt << endl;
        outFile << "張三豐,20," << dt << endl;

        outFile.close();
    }
}

// 顯示遊戲標題
void showGameTitle()
{
    clearScreen();

    int titleY = 2;

    setColor(10); // 亮綠色

    // 使用中文「貪吃蛇」標題
    gotoxy(35, titleY);
    cout << "╔═════════════════════════╗";
    gotoxy(35, titleY + 1);
    cout << "║                         ║";
    gotoxy(35, titleY + 2);
    cout << "║       貪  吃  蛇        ║";
    gotoxy(35, titleY + 3);
    cout << "║                         ║";
    gotoxy(35, titleY + 4);
    cout << "╚═════════════════════════╝";

    setColor(15); // 還原白色
}

// 顯示選單選項
void showMenuOption(int x, int y, string text, bool selected)
{
    gotoxy(x, y);
    if (selected)
    {
        setColor(11); // 淺藍色表示選中
        cout << "► " << text << " ◄";
    }
    else
    {
        setColor(7); // 灰色表示未選中
        cout << "  " << text << "  ";
    }
    setColor(15); // 還原白色
}

// 顯示遊戲規則 - 修正版
void showGameRules()
{
    clearScreen();

    setColor(14); // 黃色
    gotoxy(35, 2);
    cout << "《 遊戲規則 》";
    setColor(15); // 白色

    int ruleY = 5;
    gotoxy(30, ruleY++);
    cout << "┌─────────────────────────────────┐";
    gotoxy(30, ruleY++);
    cout << "│                                 │";
    gotoxy(30, ruleY++);
    cout << "│  1. 使用WASD鍵控制蛇的移動方向  │";
    gotoxy(30, ruleY++);
    cout << "│                                 │";
    gotoxy(30, ruleY++);
    cout << "│  2. 吃掉食物以增加蛇的長度      │";
    gotoxy(30, ruleY++);
    cout << "│                                 │";
    gotoxy(30, ruleY++);
    cout << "│  3. 避免碰到牆壁或自己的身體    │";
    gotoxy(30, ruleY++);
    cout << "│                                 │";
    gotoxy(30, ruleY++);
    cout << "│  4. 按ESC鍵退出遊戲             │";
    gotoxy(30, ruleY++);
    cout << "│                                 │";
    gotoxy(30, ruleY++);
    cout << "└─────────────────────────────────┘";

    setColor(10); // 綠色
    gotoxy(35, ruleY + 2);
    cout << "祝你好運！";

    setColor(15); // 還原白色

    gotoxy(30, ruleY + 4);
    cout << "按任意鍵返回主選單...";
    _getch();

    clearScreen(); // 返回主選單前再次清屏
}

// 添加此函數來繪製遊戲資訊面板
void drawGameInfoPanel(int score, int speed, int width, int height)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos;

    // 繪製資訊區域框架
    SetConsoleTextAttribute(hConsole, 8);
    for (int i = 0; i < width * 2 + 2; i++)
    {
        cursorPos.X = i;
        cursorPos.Y = height;
        SetConsoleCursorPosition(hConsole, cursorPos);
        cout << "─";
    }

    // 繪製資訊內容
    cursorPos.X = 2;
    cursorPos.Y = height + 1;
    SetConsoleCursorPosition(hConsole, cursorPos);
    SetConsoleTextAttribute(hConsole, 14);
    cout << "分數: " << score;

    cursorPos.X = 2;
    cursorPos.Y = height + 2;
    SetConsoleCursorPosition(hConsole, cursorPos);
    SetConsoleTextAttribute(hConsole, 11);
    cout << "速度: " << (150 - speed) / 10 + 1;

    cursorPos.X = width - 15;
    cursorPos.Y = height + 1;
    SetConsoleCursorPosition(hConsole, cursorPos);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "WASD/方向鍵: 控制方向";

    cursorPos.X = width - 15;
    cursorPos.Y = height + 2;
    SetConsoleCursorPosition(hConsole, cursorPos);
    SetConsoleTextAttribute(hConsole, 12);
    cout << "ESC: 退出遊戲";

    SetConsoleTextAttribute(hConsole, 7);
}

// 遊戲邏輯的執行函數
int runGame()
{
    // 清屏
    system("cls");

    // 初始化遊戲
    const int width = 30;
    const int height = 20;
    Gamemap gameMap(width, height);
    Snake snake;
    snake.setHead(width / 2, height / 2); // 設定蛇頭初始位置
    snake.setLength(1);                   // 初始蛇長度為1
    Food food(width - 2, height - 2);     // 食物位置在內部區域

    // 調整食物位置，確保不在牆壁上或蛇頭位置
    while (food.getX() <= 0 || food.getX() >= width - 1 ||
           food.getY() <= 0 || food.getY() >= height - 1 ||
           (food.getX() == snake.getHeadX() && food.getY() == snake.getHeadY()))
    {
        food.respawn(width - 2, height - 2);
    }

    Direction dir = STOP;
    bool gameOver = false;
    int score = 0;
    int speed = 150; // 移動速度 (毫秒)

    // 先繪製初始地圖
    gameMap.draw(snake, food);

    // 顯示漂亮的資訊面板
    drawGameInfoPanel(score, speed, width, height);

    // 顯示開始提示
    // printAt(0, height + 1, "分數: 0");
    // printAt(0, height + 2, "按任意方向鍵開始遊戲，ESC鍵退出");
    // printAt(0, height + 3, "使用 WASD 或方向鍵控制蛇的移動");

    // 等待玩家按下按鍵開始遊戲
    bool gameStarted = false;
    while (!gameStarted && !gameOver)
    {
        if (_kbhit())
        {
            int key = _getch();
            if (key == 27) // ESC鍵
            {
                gameOver = true;
                break;
            }
            else if (key == 'a' || key == 'A' || key == 75) // 左
            {
                dir = LEFT;
                gameStarted = true;
            }
            else if (key == 'd' || key == 'D' || key == 77) // 右
            {
                dir = RIGHT;
                gameStarted = true;
            }
            else if (key == 'w' || key == 'W' || key == 72) // 上
            {
                dir = UP;
                gameStarted = true;
            }
            else if (key == 's' || key == 'S' || key == 80) // 下
            {
                dir = DOWN;
                gameStarted = true;
            }
            else if (key == 224) // 方向鍵的第一個字節
            {
                key = _getch();
                if (key == 75) // 左方向鍵
                {
                    dir = LEFT;
                    gameStarted = true;
                }
                else if (key == 77) // 右方向鍵
                {
                    dir = RIGHT;
                    gameStarted = true;
                }
                else if (key == 72) // 上方向鍵
                {
                    dir = UP;
                    gameStarted = true;
                }
                else if (key == 80) // 下方向鍵
                {
                    dir = DOWN;
                    gameStarted = true;
                }
            }
        }
        Sleep(50); // 避免CPU佔用過高
    }

    // 遊戲主循環
    while (!gameOver)
    {
        // 檢查按鍵輸入 (非阻塞)
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'a':
            case 'A':
            case 75: // 左方向鍵
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 'd':
            case 'D':
            case 77: // 右方向鍵
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 'w':
            case 'W':
            case 72: // 上方向鍵
                if (dir != DOWN)
                    dir = UP;
                break;
            case 's':
            case 'S':
            case 80: // 下方向鍵
                if (dir != UP)
                    dir = DOWN;
                break;
            case 27: // ESC鍵
                gameOver = true;
                break;
            }
        }

        // 根據方向移動蛇
        int newHeadX = snake.getHeadX();
        int newHeadY = snake.getHeadY();

        switch (dir)
        {
        case LEFT:
            newHeadX--;
            break;
        case RIGHT:
            newHeadX++;
            break;
        case UP:
            newHeadY--;
            break;
        case DOWN:
            newHeadY++;
            break;
        }

        // 檢查是否碰到牆壁
        if (newHeadX <= 0 || newHeadX >= width - 1 || newHeadY <= 0 || newHeadY >= height - 1)
        {
            gameOver = true;
            continue;
        }

        // 檢查是否碰到自己身體
        for (int i = 0; i < snake.getLength(); i++)
        {
            if (newHeadX == snake.getBodyX(i) && newHeadY == snake.getBodyY(i))
            {
                gameOver = true;
                break;
            }
        }

        // 檢查是否吃到食物
        if (newHeadX == food.getX() && newHeadY == food.getY())
        {
            score++;
            // 增加蛇的長度
            int oldLength = snake.getLength();
            snake.setLength(oldLength + 1);

            // 生成新食物
            food.respawn(width - 2, height - 2);

            // 調整食物位置，確保不在牆壁上或蛇身上
            while (food.getX() <= 0 || food.getX() >= width - 1 ||
                   food.getY() <= 0 || food.getY() >= height - 1)
            {
                food.respawn(width - 2, height - 2);
            }

            // 檢查食物是否與蛇身重疊
            bool foodOnSnake = false;
            do
            {
                foodOnSnake = false;
                // 檢查與蛇頭
                if (food.getX() == snake.getHeadX() && food.getY() == snake.getHeadY())
                {
                    foodOnSnake = true;
                }

                // 檢查與蛇身
                for (int i = 0; i < snake.getLength(); i++)
                {
                    if (food.getX() == snake.getBodyX(i) && food.getY() == snake.getBodyY(i))
                    {
                        foodOnSnake = true;
                        break;
                    }
                }

                // 如果食物在蛇身上，重新生成
                if (foodOnSnake)
                {
                    food.respawn(width - 2, height - 2);
                }
            } while (foodOnSnake && food.getX() > 0 && food.getX() < width - 1 && food.getY() > 0 && food.getY() < height - 1);

            // 隨著分數增加，適當加快速度
            if (score % 5 == 0 && speed > 50)
                speed -= 10;
        }

        // 更新蛇的位置
        // 1. 儲存當前蛇的所有節點位置
        int oldX[100], oldY[100];
        oldX[0] = snake.getHeadX();
        oldY[0] = snake.getHeadY();
        for (int i = 0; i < snake.getLength(); i++)
        {
            oldX[i + 1] = snake.getBodyX(i);
            oldY[i + 1] = snake.getBodyY(i);
        }

        // 2. 更新蛇頭位置
        snake.setHead(newHeadX, newHeadY);

        // 3. 更新蛇身位置
        for (int i = 0; i < snake.getLength(); i++)
        {
            snake.setBody(i, oldX[i], oldY[i]);
        }

        // 繪製遊戲畫面
        gameMap.draw(snake, food);

        // 更新資訊面板
        drawGameInfoPanel(score, speed, width, height);

        // 顯示分數
        // printAt(0, height + 1, "分數: " + to_string(score));
        // printAt(0, height + 2, "按ESC退出遊戲");

        // 控制遊戲速度
        Sleep(speed);
    }

    // 遊戲結束
    system("cls");
    printAt(width / 2 - 5, height / 2 - 2, "遊戲結束！");
    printAt(width / 2 - 10, height / 2, "你的最終分數是: " + to_string(score));
    printAt(width / 2 - 12, height / 2 + 2, "按任意鍵返回主選單...");

    _getch();

    // 直接返回分數，不再需要臨時檔案
    return score;
}

// 儲存遊戲分數
void saveScore(int score)
{
    clearScreen();

    string playerName;
    setColor(14);
    gotoxy(35, 10);
    cout << "恭喜獲得 " << score << " 分！";
    gotoxy(35, 12);
    cout << "請輸入您的名字: ";
    setColor(15);

    // 顯示光標以便輸入名字
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    cin >> playerName;

    // 名字輸入完成後再次隱藏光標
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // 獲取當前時間作為記錄
    time_t now = time(0);
    string dt = ctime(&now);
    dt = dt.substr(0, dt.length() - 1); // 去除換行符

    // 讀取現有紀錄
    vector<ScoreRecord> records;
    ifstream inFile("scores.txt");
    if (inFile.is_open())
    {
        string line;
        while (getline(inFile, line))
        {
            ScoreRecord record;
            size_t pos1 = line.find(",");
            size_t pos2 = line.find(",", pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos)
            {
                record.playerName = line.substr(0, pos1);
                record.score = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                record.date = line.substr(pos2 + 1);
                records.push_back(record);
            }
        }
        inFile.close();
    }

    // 添加新紀錄
    ScoreRecord newRecord;
    newRecord.playerName = playerName;
    newRecord.score = score;
    newRecord.date = dt;
    records.push_back(newRecord);

    // 排序紀錄（按分數降序）
    sort(records.begin(), records.end());

    // 如果紀錄超過10筆，只保留前10筆
    if (records.size() > 10)
    {
        records.resize(10);
    }

    // 寫入紀錄到檔案
    ofstream outFile("scores.txt");
    if (outFile.is_open())
    {
        for (const auto &record : records)
        {
            outFile << record.playerName << "," << record.score << "," << record.date << endl;
        }
        outFile.close();
    }

    // 顯示得分儲存成功訊息
    gotoxy(35, 14);
    setColor(10);
    cout << "分數已儲存！";
    gotoxy(35, 16);
    setColor(15);
    cout << "按任意鍵繼續...";
    _getch();
}

// 顯示高分榜
void showHighScores()
{
    clearScreen();

    setColor(14);
    gotoxy(35, 2);
    cout << "《 高分榜 》";
    setColor(15);

    // 讀取紀錄
    vector<ScoreRecord> records;
    ifstream inFile("scores.txt");
    if (inFile.is_open())
    {
        string line;
        while (getline(inFile, line))
        {
            ScoreRecord record;
            size_t pos1 = line.find(",");
            size_t pos2 = line.find(",", pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos)
            {
                record.playerName = line.substr(0, pos1);
                record.score = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                record.date = line.substr(pos2 + 1);
                records.push_back(record);
            }
        }
        inFile.close();
    }

    // 排序紀錄（按分數降序）
    sort(records.begin(), records.end());

    // 繪製表格
    int tableY = 5;
    gotoxy(20, tableY);
    cout << "┌──────┬────────────────┬─────────┬─────────────────────────────┐";
    gotoxy(20, tableY + 1);
    cout << "│ 排名 │     玩家       │  分數   │           日期              │";
    gotoxy(20, tableY + 2);
    cout << "├──────┼────────────────┼─────────┼─────────────────────────────┤";

    // 顯示紀錄
    for (size_t i = 0; i < records.size(); i++)
    {
        gotoxy(20, tableY + 3 + i);
        cout << "│  " << i + 1;
        if (i < 9)
            cout << "   ";
        else
            cout << "  ";

        cout << "│ " << records[i].playerName;
        // 填充空格使表格對齊
        for (size_t j = records[i].playerName.length(); j < 14; j++)
        {
            cout << " ";
        }

        cout << "│  " << records[i].score;
        // 填充空格使表格對齊
        for (size_t j = to_string(records[i].score).length(); j < 5; j++)
        {
            cout << " ";
        }

        cout << "│ " << records[i].date;
        // 填充空格使表格對齊
        for (size_t j = records[i].date.length(); j < 25; j++)
        {
            cout << " ";
        }
        cout << "│";
    }

    // 如果沒有紀錄，顯示相應訊息
    if (records.empty())
    {
        gotoxy(20, tableY + 3);
        cout << "│                    尚無遊戲紀錄                             │";
    }

    // 繪製表格底部
    gotoxy(20, tableY + 3 + (records.empty() ? 0 : records.size()));
    cout << "└──────┴────────────────┴─────────┴─────────────────────────────┘";

    setColor(15);
    gotoxy(35, tableY + 15);
    cout << "按任意鍵返回主選單...";
    _getch();
}

// 這個函數會執行遊戲並且返回玩家獲得的分數
int playGame()
{
    clearScreen();
    gotoxy(40, 15);
    setColor(10);
    cout << "遊戲開始！";
    Sleep(1000);

    // 執行遊戲，runGame 函數現在直接返回分數
    int score = runGame();

    // 確保分數有效
    if (score <= 0)
    {
        score = 10; // 預設分數
        clearScreen();
        gotoxy(30, 12);
        setColor(14);
        cout << "無法獲取有效分數，使用預設分數: " << score;
        gotoxy(30, 14);
        setColor(15);
        cout << "按任意鍵繼續...";
        _getch();
    }

    return score;
}

int main()
{
    // 設定控制台窗口屬性
    system("title 貪吃蛇遊戲");
    system("mode con cols=100 lines=30");

    // 清空整個螢幕
    clearScreen();

    // 建立預設紀錄，如果紀錄檔不存在
    createDefaultRecords();

    // 隱藏光標
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    int selectedOption = 0;
    const int OPTION_COUNT = 4; // 增加了一個選項：高分榜
    bool exitGame = false;

    while (!exitGame)
    {
        showGameTitle();

        setColor(15); // 白色
        gotoxy(32, 10);
        cout << "▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂";

        gotoxy(40, 12);
        cout << "功能選項:";

        // 顯示選單選項
        showMenuOption(40, 14, "開始遊戲", selectedOption == 0);
        showMenuOption(40, 15, "遊戲規則", selectedOption == 1);
        showMenuOption(40, 16, "高分榜", selectedOption == 2);
        showMenuOption(40, 17, "離開遊戲", selectedOption == 3);

        gotoxy(32, 19);
        cout << "▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂▂";

        gotoxy(30, 21);
        setColor(8); // 暗灰色
        cout << "使用 ↑↓ 鍵選擇選項，按 Enter 確認";

        // 刷新時間顯示
        time_t now = time(0);
        string dt = ctime(&now);
        gotoxy(35, 26);
        setColor(7);
        cout << dt.substr(0, dt.length() - 1);

        // 取得鍵盤輸入
        int key = _getch();

        if (key == 224)
        { // 如果是特殊鍵
            key = _getch();

            if (key == 72)
            { // 上箭頭
                selectedOption = (selectedOption > 0) ? selectedOption - 1 : OPTION_COUNT - 1;
            }
            else if (key == 80)
            { // 下箭頭
                selectedOption = (selectedOption + 1) % OPTION_COUNT;
            }
        }
        else if (key == 13)
        { // Enter鍵
            switch (selectedOption)
            {
            case 0: // 開始遊戲
            {
                int score = playGame();
                if (score > 0)
                {
                    saveScore(score);
                }
            }
            break;
            case 1: // 遊戲規則
                showGameRules();
                break;
            case 2: // 高分榜
                showHighScores();
                break;
            case 3: // 離開遊戲
                exitGame = true;
                break;
            }
        }
    }

    clearScreen();
    gotoxy(35, 12);
    setColor(14);
    cout << "感謝您的遊玩！再見！";
    gotoxy(35, 14);
    cout << "按任意鍵退出...";

    _getch();
    return 0;
}