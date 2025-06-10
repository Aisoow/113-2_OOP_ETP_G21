# 🐍 C++ 貪吃蛇遊戲程式碼詳細教學

## 📋 專案架構概覽

您的貪吃蛇遊戲採用了物件導向設計，包含以下核心類別：

```
專案結構:
├── Snake.h/cpp      # 蛇類別
├── Food.h/cpp       # 食物類別  
├── Gamemap.h/cpp    # 遊戲地圖類別
└── game.cpp         # 主程式
```

## 🔧 核心類別詳解

### 1. Snake 類別 - 蛇的實作

#### 標頭檔 (Snake.h)
````cpp
class Snake {
private:
    int headX, headY;           // 蛇頭坐標
    int bodyX[100], bodyY[100]; // 蛇身坐標陣列
    int length;                 // 蛇身長度

public:
    Snake();                    // 建構子
    void setHead(int x, int y); // 設定蛇頭位置
    // ... 其他方法
};
````

**設計要點:**
- 使用**封裝原則**：私有成員變數，公有介面方法
- 固定大小陣列 `bodyX[100], bodyY[100]` 儲存蛇身坐標
- 限制最大長度為100節，避免記憶體溢出

#### 實作檔 (Snake.cpp)
````cpp
Snake::Snake() {
    headX = 0; headY = 0; length = 0;
    // 初始化所有蛇身坐標為0
    for (int i = 0; i < 100; i++) {
        bodyX[i] = 0; bodyY[i] = 0;
    }
}
````

**關鍵方法分析:**
- `setBody()`: 包含**邊界檢查**，防止陣列越界
- `getBodyX()/getBodyY()`: 返回-1表示無效索引，提供**錯誤處理**

### 2. Food 類別 - 食物系統

#### 設計特色
````cpp
Food::Food(int width, int height) {
    std::srand(static_cast<unsigned>(std::time(0))); // 設定隨機種子
    respawn(width, height);
}

void Food::respawn(int width, int height) {
    x = std::rand() % width;  // 隨機X坐標
    y = std::rand() % height; // 隨機Y坐標
}
````

**重要概念:**
- **RAII原則**: 建構子中初始化隨機種子
- `const` 修飾的getter方法，保證不修改物件狀態
- 簡潔的介面設計

### 3. Gamemap 類別 - 遊戲地圖核心

#### 動態記憶體管理
````cpp
class Gamemap {
private:
    int **map;  // 二維動態陣列指標
    void allocateMap();   // 分配記憶體
    void deallocateMap(); // 釋放記憶體
};
````

**記憶體管理實作:**
````cpp
void Gamemap::allocateMap() {
    map = new int*[width];        // 分配一維指標陣列
    for (int i = 0; i < width; i++) {
        map[i] = new int[height]; // 分配每行的記憶體
    }
}

void Gamemap::deallocateMap() {
    if (map == nullptr) return;
    for (int i = 0; i < width; i++) {
        delete[] map[i];          // 釋放每行記憶體
    }
    delete[] map;                 // 釋放指標陣列
    map = nullptr;
}
````

**設計模式應用:**
- **RAII**: 建構子分配，解構子釋放
- **防禦性程式設計**: 檢查nullptr避免重複釋放

#### 地圖元素枚舉
````cpp
enum MapElement {
    EMPTY = 0,  // 空格
    WALL = 1,   // 牆壁
    BODY = 2,   // 蛇身
    HEAD = 3,   // 蛇頭
    FOOD = 4    // 食物
};
````

### 4. 彩色控制台輸出系統

#### Windows API 應用
````cpp
void Gamemap::printMap() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cursorPos.X = j * 2;  // 每個圖形佔2字元寬
            cursorPos.Y = i;
            SetConsoleCursorPosition(hConsole, cursorPos);
            
            switch (map[j][i]) {
                case WALL:
                    SetConsoleTextAttribute(hConsole, 119); // 藍底白字
                    cout << "[]";
                    break;
                case HEAD:
                    SetConsoleTextAttribute(hConsole, 46);  // 黃底紅字
                    cout << "◎";
                    break;
                // ... 其他情況
            }
        }
    }
}
````

**技術重點:**
- 使用 `SetConsoleCursorPosition()` 精確定位
- `SetConsoleTextAttribute()` 設定顏色
- 中文字符顯示需要適當的編碼設定

## 🎮 主程式遊戲邏輯

### 遊戲狀態管理
````cpp
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

int runGame() {
    // 初始化階段
    Gamemap gameMap(width, height);
    Snake snake;
    Food food(width-2, height-2);
    
    Direction dir = STOP;
    bool gameOver = false;
    int score = 0;
    
    // 主遊戲迴圈
    while (!gameOver) {
        // 1. 輸入處理
        if (_kbhit()) {
            // 處理鍵盤輸入
        }
        
        // 2. 遊戲邏輯更新
        // 3. 碰撞檢測
        // 4. 畫面繪製
        // 5. 速度控制
    }
}
````

### 蛇的移動算法
````cpp
// 儲存當前所有節點位置
int oldX[100], oldY[100];
oldX[0] = snake.getHeadX();
oldY[0] = snake.getHeadY();
for (int i = 0; i < snake.getLength(); i++) {
    oldX[i+1] = snake.getBodyX(i);
    oldY[i+1] = snake.getBodyY(i);
}

// 更新蛇頭到新位置
snake.setHead(newHeadX, newHeadY);

// 蛇身跟隨：每節移到前一節的舊位置
for (int i = 0; i < snake.getLength(); i++) {
    snake.setBody(i, oldX[i], oldY[i]);
}
````

**算法分析:**
- 採用**位置記錄法**，先保存所有位置
- 蛇頭移動到新位置，蛇身依序跟隨
- 時間複雜度：O(n)，n為蛇的長度

### 碰撞檢測系統
````cpp
// 牆壁碰撞
if (newHeadX <= 0 || newHeadX >= width-1 || 
    newHeadY <= 0 || newHeadY >= height-1) {
    gameOver = true;
}

// 自身碰撞
for (int i = 0; i < snake.getLength(); i++) {
    if (newHeadX == snake.getBodyX(i) && 
        newHeadY == snake.getBodyY(i)) {
        gameOver = true;
        break;
    }
}

// 食物碰撞
if (newHeadX == food.getX() && newHeadY == food.getY()) {
    score++;
    snake.setLength(snake.getLength() + 1);
    food.respawn(width-2, height-2);
}
````

## 📊 資料持久化系統

### 高分記錄結構
````cpp
struct ScoreRecord {
    string playerName;
    int score;
    string date;
    
    // 自定義比較運算子用於排序
    bool operator<(const ScoreRecord& other) const {
        return score > other.score;  // 降序排列
    }
};
````

### 檔案I/O操作
````cpp
void saveScore(int score) {
    // 讀取現有記錄
    vector<ScoreRecord> records;
    ifstream inFile("scores.txt");
    
    // 解析CSV格式資料
    string line;
    while (getline(inFile, line)) {
        size_t pos1 = line.find(",");
        size_t pos2 = line.find(",", pos1 + 1);
        
        ScoreRecord record;
        record.playerName = line.substr(0, pos1);
        record.score = stoi(line.substr(pos1+1, pos2-pos1-1));
        record.date = line.substr(pos2+1);
        records.push_back(record);
    }
    
    // 添加新記錄並排序
    records.push_back(newRecord);
    sort(records.begin(), records.end());
    
    // 寫回檔案
    ofstream outFile("scores.txt");
    for (const auto& record : records) {
        outFile << record.playerName << "," 
                << record.score << "," 
                << record.date << endl;
    }
}
````

## 🎨 使用者介面設計

### 選單系統
````cpp
void showMenuOption(int x, int y, string text, bool selected) {
    gotoxy(x, y);
    if (selected) {
        setColor(11);  // 淺藍色表示選中
        cout << "► " << text << " ◄";
    } else {
        setColor(7);   // 灰色表示未選中
        cout << "  " << text << "  ";
    }
}
````

### 遊戲資訊面板
````cpp
void drawGameInfoPanel(int score, int speed, int width, int height) {
    // 繪製分隔線
    for (int i = 0; i < width * 2 + 2; i++) {
        gotoxy(i, height);
        cout << "─";
    }
    
    // 顯示遊戲資訊
    gotoxy(2, height + 1);
    cout << "分數: " << score;
    
    gotoxy(2, height + 2);
    cout << "速度: " << (150 - speed) / 10 + 1;
}
````

## 🔍 程式設計模式與最佳實踐

### 1. 物件導向原則
- **封裝**: 類別內部資料隱藏，透過公有介面存取
- **單一職責**: 每個類別負責特定功能
- **開放封閉**: 可擴展但不修改現有程式碼

### 2. 記憶體管理
- **RAII**: 資源獲取即初始化
- **異常安全**: 解構子中釋放資源
- **防禦性程式設計**: 檢查指標有效性

### 3. 錯誤處理
````cpp
int Gamemap::getMap(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return map[x][y];
    }
    return WALL;  // 超出邊界返回牆壁
}
````

### 4. 效能最佳化
- 避免不必要的 `system("cls")` 呼叫
- 使用精確的游標定位減少重繪
- 適當的延遲控制降低CPU使用率

## 🚀 編譯與執行

### 編譯指令
````bash
g++ -o SnakeGame.exe game.cpp Snake.cpp Food.cpp Gamemap.cpp
````

### 執行環境設定
````cmd
chcp 65001        # 設定UTF-8編碼
SnakeGame.exe     # 執行遊戲
````

## 💡 擴展建議

### 初級改進
1. **暫停功能**: 添加空白鍵暫停遊戲
2. **邊界穿越**: 讓蛇從一邊穿越到另一邊
3. **多種食物**: 不同食物有不同分數

### 中級改進
1. **智慧指標**: 使用 `std::unique_ptr` 管理動態記憶體
2. **設計模式**: 實作觀察者模式處理遊戲事件
3. **配置檔案**: 使用JSON/XML儲存遊戲設定

### 高級改進
1. **圖形介面**: 整合SFML或SDL圖形庫
2. **網路功能**: 多人連線對戰
3. **AI系統**: 實作自動遊玩的AI

## 🎯 學習重點總結

通過這個專案，您可以學習到：

1. **C++物件導向程式設計**的實際應用
2. **動態記憶體管理**的正確方式
3. **檔案I/O操作**和資料持久化
4. **Windows API**在控制台程式中的使用
5. **遊戲開發**的基本概念和技巧
6. **程式架構設計**和模組化思維

這個貪吃蛇遊戲是一個優秀的學習專案，展示了紮實的程式設計基礎和良好的程式設計習慣！