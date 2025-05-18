#ifndef FOOD_H
#define FOOD_H
class Food
{
private:
    int x, y;

public:
    Food(int width, int height);
    void respawn(int width, int height);
    int getX() const;
    int getY() const;
    ~Food();
};
#endif // !FOOD_H
