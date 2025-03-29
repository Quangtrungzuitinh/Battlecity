#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL2/SDL.h>

class Obstacle {
private:
    int x, y, width, height;
public:
    Obstacle(int startX, int startY, int w, int h);
    void draw(SDL_Renderer *renderer);
    bool checkCollision(int objX, int objY, int objSize) const;
};

#endif
