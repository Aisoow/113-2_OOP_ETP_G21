#include "Food.h"
#include <cstdlib>
#include <ctime>

Food::Food(int width, int height) {
    std::srand(static_cast<unsigned>(std::time(0)));
    respawn(width, height);
}

void Food::respawn(int width, int height) {
    x = std::rand() % width;
    y = std::rand() % height;
}

int Food::getX() const {
    return x;
}

int Food::getY() const {
    return y;
}
