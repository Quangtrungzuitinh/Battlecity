#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Obstacle.h"

const int ENEMY_SIZE = 40;

class Enemy {
private:
    int x, y;
    int speed;
    int direction; // Declare direction to store movement direction
    SDL_Texture* texture; // Thêm texture

public:
    Enemy(int startX, int startY, int spd, SDL_Renderer* renderer);
    ~Enemy(); // Destructor để giải phóng texture

    void Render(SDL_Renderer* renderer);

    void move();

    void draw(SDL_Renderer *renderer);

    int getX() const { return x; }
    int getY() const { return y; }

    void changeDirection();
    void move(const std::vector<Obstacle> &obstacles);

    int getDirection() const; // Move this method declaration to the public section.
};

#endif