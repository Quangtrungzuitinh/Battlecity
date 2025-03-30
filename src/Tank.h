#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Obstacle.h"

const int TANK_SIZE = 40;

class Tank {
private:
    int x, y;
    int speed;
    int direction;
    SDL_Texture* texture; // Thêm texture

public:
    Tank(int startX, int startY, int spd, SDL_Renderer* renderer);
    ~Tank(); // Destructor để giải phóng texture
    void setPosition(int x, int y);

    int getDirection() const { return direction; }

    void move(SDL_Event &e);

    void draw(SDL_Renderer *renderer);
    void move(SDL_Event &event, const std::vector<Obstacle> &obstacles);

    int getX() const { return x; }
    int getY() const { return y; }
};
#endif