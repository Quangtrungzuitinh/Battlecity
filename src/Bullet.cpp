#include "Bullet.h"

Bullet::Bullet(int startX, int startY, int spd, int dir) : x(startX), y(startY), speed(spd), direction(dir), active(true) {}

void Bullet::move() {
    if (!active) return;

    switch (direction) {
        case 0: y -= speed; break; // Up
        case 1: y += speed; break; // Down
        case 2: x -= speed; break; // Left
        case 3: x += speed; break; // Right
    }

    // Deactivate bullet if it goes out of bounds
    if (x < 0 || y < 0 || x > 800 || y > 600) {
        active = false;
    }
}

void Bullet::draw(SDL_Renderer *renderer) {
    if (!active) return;

    SDL_Rect bulletRect = {x, y, BULLET_SIZE, BULLET_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bulletRect);
}
