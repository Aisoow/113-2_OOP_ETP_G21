#ifndef GAMEMAP_H
#define GAMEMAP_H
class Gamemap
{
private:
    int width;
    int height;
    bool **map;              // 地圖二元陣列 先以這個的邊緣陣列內容作邊界 後面有用到border再改 或是需要新增場內柱子再更改
    bool **border;           // 邊界二元陣列 之後新增功能可能有用
    void allocateMap();      // 分配map的記憶體
    void deallocateMap();    // 釋放map的記憶體
    void allocateBorder();   // 分配border的記憶體
    void deallocateBorder(); // 釋放border的記憶體
public:
    Gamemap();
    Gamemap(int w, int h);                 // 建構子
    void setWidth(int w);                  // 設定地圖寬度
    void setHeight(int h);                 // 設定地圖高度
    int getWidth();                        // 取得地圖寬度
    int getHeight();                       // 取得地圖高度
    void setMap(int x, int y, bool value); // 設定地圖特定區域的值
    bool getMap(int x, int y);             // 取得地圖特定區域的值
    void setBorder(int x, int y);          // 設定邊界
    bool getBorder(int x, int y);          // 設定邊界
    void printMap();                       // 印出地圖
    void printBorder();                    // 印出邊界
    void clearMap();                       // 清除地圖
    void clearBorder();                    // 清除邊界
    void initMap();                        // 初始化地圖
};
#endif // !GAMEMAP_H