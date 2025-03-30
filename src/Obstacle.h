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
    // Thêm các phương thức getter
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif
