#include "Enemy.h"
#include <cstdlib>
#include <memory>  // ✅ Thêm thư viện memory

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::shared_ptr<SDL_Texture> Enemy::texture = nullptr;  // ✅ Khai báo texture dùng chung

Enemy::Enemy(int startX, int startY, int spd, SDL_Renderer* renderer) : x(startX), y(startY), speed(spd) {
    direction = rand() % 4; // Ngẫu nhiên hướng di chuyển ban đầu
    
    // ✅ Tạo texture nếu chưa có
    if (!texture) {
        SDL_Surface* surface = IMG_Load("assets/enemy.png");
        if (!surface) {
            SDL_Log("Không thể tải enemy.png: %s", IMG_GetError());
        } else {
            texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
            SDL_FreeSurface(surface);
        }
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

    for (const auto &obs : obstacles) {
        if (obs.checkCollision(newX, newY, ENEMY_SIZE)) {
            changeDirection();
            return;
        }
    }

    if (newX < 0 || newX + ENEMY_SIZE > SCREEN_WIDTH || newY < 0 || newY + ENEMY_SIZE > SCREEN_HEIGHT) {
        direction = rand() % 4;
    } else {
        x = newX;
        y = newY;
    }
}

void Enemy::draw(SDL_Renderer *renderer) {
    SDL_Rect enemyRect = {x, y, ENEMY_SIZE, ENEMY_SIZE};
    if (texture) {
        SDL_RenderCopy(renderer, texture.get(), nullptr, &enemyRect);
    } else {
        SDL_Log("Enemy texture is NULL!");
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
Enemy::~Enemy() {

}
