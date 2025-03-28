#include "Tank.h"

Tank::Tank(int startX, int startY, int spd) : x(startX), y(startY), speed(spd) {}

void Tank::move(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:    y -= speed; direction = 0; break;
            case SDLK_DOWN:  y += speed; direction = 1; break;
            case SDLK_LEFT:  x -= speed; direction = 2; break;
            case SDLK_RIGHT: x += speed; direction = 3; break;
        }
    }
}


void Tank::draw(SDL_Renderer *renderer) {
    SDL_Rect tankRect = {x, y, TANK_SIZE, TANK_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &tankRect);
}
