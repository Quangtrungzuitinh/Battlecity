#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

const int BULLET_SIZE = 8;

class Bullet {
private:
    int x, y;
    int speed;
    bool active;
    int direction; // 0: Up, 1: Down, 2: Left, 3: Right

public:
    Bullet(int startX, int startY, int spd, int dir);

    void move();

    void draw(SDL_Renderer *renderer);

    bool isActive() const { return active; }

    int getX() const { return x; }
    int getY() const { return y; }
};

#endif
