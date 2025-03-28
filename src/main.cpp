#include <SDL2/SDL.h>
#include "Tank.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <algorithm>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_LIVES = 7;

// Hàm kiểm tra va chạm giữa Bullet và Tank
bool checkCollision(const Bullet &b, const Tank &t) {
    return (b.getX() < t.getX() + TANK_SIZE &&
            b.getX() + BULLET_SIZE > t.getX() &&
            b.getY() < t.getY() + TANK_SIZE &&
            b.getY() + BULLET_SIZE > t.getY());
}

// Hàm kiểm tra va chạm giữa Bullet và Enemy
bool checkCollision(const Bullet &b, const Enemy &e) {
    return (b.getX() < e.getX() + ENEMY_SIZE &&
            b.getX() + BULLET_SIZE > e.getX() &&
            b.getY() < e.getY() + ENEMY_SIZE &&
            b.getY() + BULLET_SIZE > e.getY());
}

// Hàm vẽ số mạng còn lại ở góc dưới bên phải
void drawLives(SDL_Renderer *renderer, int lives) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    int iconSize = 20;
    int padding = 5;
    int startX = SCREEN_WIDTH - (lives * (iconSize + padding));
    int startY = SCREEN_HEIGHT - 30;

    for (int i = 0; i < lives; ++i) {
        SDL_Rect lifeRect = {startX + i * (iconSize + padding), startY, iconSize, iconSize};
        SDL_RenderFillRect(renderer, &lifeRect);
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event e;

    Tank player(400, 500, 5);
    std::vector<Bullet> bullets;
    std::vector<Bullet> enemyBullets;
    std::vector<Enemy> enemies = {Enemy(100, 100, 2), Enemy(600, 100, 2)};

    int playerLives = PLAYER_LIVES;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            player.move(e);

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                bullets.emplace_back(player.getX() + TANK_SIZE / 2 - BULLET_SIZE / 2, player.getY(), 8, player.getDirection());
            }
        }

        for (auto &bullet : bullets) {
            bullet.move();
        }

        for (auto &enemy : enemies) {
            enemy.move();
            if (rand() % 100 < 2) {
                enemyBullets.emplace_back(enemy.getX() + ENEMY_SIZE / 2 - BULLET_SIZE / 2, enemy.getY(), 6, enemy.getDirection());
            }
        }

        for (auto &bullet : enemyBullets) {
            bullet.move();
        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet &b) {
            return b.getX() < 0 || b.getX() > SCREEN_WIDTH || b.getY() < 0 || b.getY() > SCREEN_HEIGHT;
        }), bullets.end());

        enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const Bullet &b) {
            return b.getX() < 0 || b.getX() > SCREEN_WIDTH || b.getY() < 0 || b.getY() > SCREEN_HEIGHT;
        }), enemyBullets.end());

        for (auto bullet = bullets.begin(); bullet != bullets.end();) {
            bool bulletRemoved = false;
            for (auto enemy = enemies.begin(); enemy != enemies.end();) {
                if (checkCollision(*bullet, *enemy)) {
                    enemy = enemies.erase(enemy);
                    bullet = bullets.erase(bullet);
                    bulletRemoved = true;
                    break;
                } else {
                    ++enemy;
                }
            }
            if (!bulletRemoved) {
                ++bullet;
            }
        }

        for (auto bullet = enemyBullets.begin(); bullet != enemyBullets.end();) {
            if (checkCollision(*bullet, player)) {
                bullet = enemyBullets.erase(bullet);
                playerLives--;
                if (playerLives <= 0) {
                    running = false;
                }
            } else {
                ++bullet;
            }
        }

        if (enemies.empty()) {
            running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        player.draw(renderer);
        for (auto &bullet : bullets) {
            bullet.draw(renderer);
        }
        for (auto &bullet : enemyBullets) {
            bullet.draw(renderer);
        }
        for (auto &enemy : enemies) {
            enemy.draw(renderer);
        }

        drawLives(renderer, playerLives);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
