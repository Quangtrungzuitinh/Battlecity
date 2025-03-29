#include "Enemy.h"
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Enemy::Enemy(int startX, int startY, int spd, SDL_Renderer* renderer) : x(startX), y(startY), speed(spd), texture(nullptr) {
    direction = rand() % 4; // Ngẫu nhiên hướng di chuyển ban đầu
    
    SDL_Surface* surface = IMG_Load("assets/enemy.png");
    if (!surface) {
        SDL_Log("Không thể tải enemy.png: %s", IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

Enemy::~Enemy() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Enemy::move(const std::vector<Obstacle> &obstacles) {
    int newX = x, newY = y;

    switch (direction) {
        case 0: newY -= speed; break;
        case 1: newY += speed; break;
        case 2: newX -= speed; break;
        case 3: newX += speed; break;
    }

    // Kiểm tra va chạm với chướng ngại vật
    for (const auto &obs : obstacles) {
        if (obs.checkCollision(newX, newY, ENEMY_SIZE)) {
            changeDirection();
            return;
        }
    }
    if (newX < 0 || newX + ENEMY_SIZE > SCREEN_WIDTH ||
        newY < 0 || newY + ENEMY_SIZE > SCREEN_HEIGHT) {
        direction = rand() % 4; // Chọn hướng mới
    } else {
        x = newX;
        y = newY;
    }

    x = newX;
    y = newY;
}

void Enemy::draw(SDL_Renderer *renderer) {
    SDL_Rect enemyRect = {x, y, ENEMY_SIZE, ENEMY_SIZE};
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &enemyRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemyRect);
    }
}

void Enemy::changeDirection() {
    direction = rand() % 4;
}

int Enemy::getDirection() const {
    return direction;
}
