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
    Gamemap(int w, int h);
    void setWidth(int w);
    void setHeight(int h);
    int getWidth();
    int getHeight();
    void setMap(int x, int y, bool value);
    bool getMap(int x, int y);
    void setBorder(int x, int y);
    bool getBorder(int x, int y);
    void printMap();
    void printBorder();
    void clearMap();
    void clearBorder();
    void initMap(); // 初始化地圖
};
#endif // !GAMEMAP_H