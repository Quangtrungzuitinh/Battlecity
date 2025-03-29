#include "Obstacle.h"

Obstacle::Obstacle(int startX, int startY, int w, int h)
    : x(startX), y(startY), width(w), height(h) {}

void Obstacle::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255); // Màu nâu

    int barWidth = width / 5;  // Độ rộng cột
    int middleHeight = height / 3;  // Chiều cao thanh ngang

    SDL_Rect leftBar = {x, y, barWidth, height};  // Cột trái
    SDL_Rect rightBar = {x + width - barWidth, y, barWidth, height};  // Cột phải
    SDL_Rect middleBar = {x, y + (height / 2) - (middleHeight / 2), width, middleHeight};  // Thanh ngang

    SDL_RenderFillRect(renderer, &leftBar);
    SDL_RenderFillRect(renderer, &rightBar);
    SDL_RenderFillRect(renderer, &middleBar);
}


bool Obstacle::checkCollision(int objX, int objY, int objSize) const {
    return (objX < x + width &&
            objX + objSize > x &&
            objY < y + height &&
            objY + objSize > y);
}
