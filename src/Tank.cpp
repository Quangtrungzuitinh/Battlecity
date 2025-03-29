#include "Tank.h"

Tank::Tank(int startX, int startY, int spd) : x(startX), y(startY), speed(spd) {}

void Tank::move(SDL_Event &event, const std::vector<Obstacle> &obstacles) {
    int newX = x, newY = y;

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:    newY -= speed; direction = 0; break;
            case SDLK_DOWN:  newY += speed; direction = 1; break;
            case SDLK_LEFT:  newX -= speed; direction = 2; break;
            case SDLK_RIGHT: newX += speed; direction = 3; break;
        }
    }

    // Kiểm tra va chạm với chướng ngại vật trước khi cập nhật vị trí
    bool canMove = true;
    for (const auto &obs : obstacles) {
        if (obs.checkCollision(newX, newY, TANK_SIZE)) {
            canMove = false;
            break;
        }
    }

    if (canMove) {
        x = newX;
        y = newY;
    }
}



void Tank::draw(SDL_Renderer *renderer) {
    SDL_Rect tankRect = {x, y, TANK_SIZE, TANK_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &tankRect);
}
