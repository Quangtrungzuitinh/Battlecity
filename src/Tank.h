#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include <vector>
#include "Obstacle.h"

const int TANK_SIZE = 40;

class Tank {
private:
    int x, y;
    int speed;
    int direction;

public:
    Tank(int startX, int startY, int spd);

    int getDirection() const { return direction; }

    void move(SDL_Event &e);

    void draw(SDL_Renderer *renderer);
    void move(SDL_Event &event, const std::vector<Obstacle> &obstacles);

    int getX() const { return x; }
    int getY() const { return y; }
};

#endif
