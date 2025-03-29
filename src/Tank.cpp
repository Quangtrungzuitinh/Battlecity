#include "Tank.h"
#include <SDL.h>
#include <SDL_image.h>

Tank::Tank(int startX, int startY, int spd, SDL_Renderer* renderer)
    : x(startX), y(startY), speed(spd), texture(nullptr) {

    // Tải texture cho Tank
    SDL_Surface* surface = IMG_Load("assets/player.png");
    if (!surface) {
        SDL_Log("Không thể tải player.png: %s", IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

Tank::~Tank() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Tank::move(SDL_Event& event, const std::vector<Obstacle>& obstacles) {
    int newX = x, newY = y;

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:    newY -= speed; direction = 0; break;
            case SDLK_DOWN:  newY += speed; direction = 1; break;
            case SDLK_LEFT:  newX -= speed; direction = 2; break;
            case SDLK_RIGHT: newX += speed; direction = 3; break;
        }
    }

    // Kiểm tra va chạm với chướng ngại vật
    bool canMove = true;
    for (const auto& obs : obstacles) {
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

void Tank::draw(SDL_Renderer* renderer) {
    SDL_Rect tankRect = {x, y, TANK_SIZE, TANK_SIZE};
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &tankRect);
    }
}
