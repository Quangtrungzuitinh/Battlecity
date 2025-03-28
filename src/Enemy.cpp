#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(int startX, int startY, int spd) : x(startX), y(startY), speed(spd) {
    direction = rand() % 4; // Ngẫu nhiên hướng di chuyển ban đầu
}

void Enemy::move() {
    switch (direction) {
        case 0: y -= speed; break; // Up
        case 1: y += speed; break; // Down
        case 2: x -= speed; break; // Left
        case 3: x += speed; break; // Right
    }

    // Đổi hướng nếu ra khỏi màn hình
    if (x < 0 || y < 0 || x > 800 - ENEMY_SIZE || y > 600 - ENEMY_SIZE) {
        changeDirection();
    }
}

void Enemy::draw(SDL_Renderer *renderer) {
    SDL_Rect enemyRect = {x, y, ENEMY_SIZE, ENEMY_SIZE};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &enemyRect);
}

void Enemy::changeDirection() {
    direction = rand() % 4;
}

int Enemy::getDirection() const {
    return direction;
}
