#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Tank.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <string>        
#include <algorithm>
#include "obstacle.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_LIVES = 7;
    int obstacleWidth = 500;
    int obstacleHeight = 250;
    int obstacleX = (SCREEN_WIDTH - obstacleWidth) / 2;
    int obstacleY = (SCREEN_HEIGHT - obstacleHeight) / 2;

    bool showMenu(SDL_Renderer* renderer) {
        SDL_Texture* menuTexture = IMG_LoadTexture(renderer, "C:/Users/ACER/Documents/Tap code/GameGem/assets/startmenu.png");
        if (!menuTexture) {
            return false;
        }
    
        bool inMenu = true;
        SDL_Event event;
        while (inMenu) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_DestroyTexture(menuTexture);
                    return false;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        SDL_DestroyTexture(menuTexture);
                        return true; // Bắt đầu game
                    }
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        SDL_DestroyTexture(menuTexture);
                        return false; // Thoát
                    }
                }
            }
    
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
        SDL_DestroyTexture(menuTexture);
        return false;
    }
    
    bool showGameOver(SDL_Renderer* renderer) {
        SDL_Texture* gameOverTexture = IMG_LoadTexture(renderer, "C:/Users/ACER/Documents/Tap code/GameGem/assets/gameover.png");
        if (!gameOverTexture) {
            return false;
        }
    
        Uint32 startTime = SDL_GetTicks(); // Lưu thời điểm bắt đầu hiển thị
        bool inGameOver = true;
        SDL_Event event;
    
        while (inGameOver) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_DestroyTexture(gameOverTexture);
                    return false;
                }
            }
    
            // Thoát sau 5 giây (5000 ms)
            if (SDL_GetTicks() - startTime > 5000) {
                inGameOver = false;
            }
    
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
    
        SDL_DestroyTexture(gameOverTexture);
        return false; // Thoát game
    }
    
    



        
    std::vector<Obstacle> obstacles = {
        Obstacle((SCREEN_WIDTH - 500) / 2, obstacleY, 500, 30), // Thanh ngang trên giữa
        Obstacle((SCREEN_WIDTH - 500) / 2, obstacleY + obstacleHeight - 30, 500, 30), // Thanh ngang dưới giữa
        Obstacle(obstacleX + (obstacleWidth / 2) - 15, obstacleY + 30, 30, obstacleHeight - 60) // Thanh dọc giữa
    };
    
    bool running = true;
    SDL_Event event;

    
    
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

// Hàm kiểm tra va chạm giữa Bullet và Obstacle
bool checkCollision(const Bullet &b, const Obstacle &o) {
    return o.checkCollision(b.getX(), b.getY(), BULLET_SIZE);
}

SDL_Texture* loadTexture(const std::string &path, SDL_Renderer *renderer) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Lỗi khi load ảnh: " << path << " - " << IMG_GetError() ;
    }
    return texture;
}


// Hàm vẽ số mạng còn lại bằng hình ảnh trái tim
void drawLives(SDL_Renderer *renderer, SDL_Texture *heartTexture, int lives) {
    int iconSize = 30;   // Kích thước của ảnh trái tim
    int padding = 5;     // Khoảng cách giữa các icon
    int startX = SCREEN_WIDTH - (lives * (iconSize + padding));
    int startY = SCREEN_HEIGHT - iconSize - 10; // Dịch lên một chút để không sát mép

    for (int i = 0; i < lives; ++i) {
        SDL_Rect heartRect = {startX + i * (iconSize + padding), startY, iconSize, iconSize};
        SDL_RenderCopy(renderer, heartTexture, nullptr, &heartRect);
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event e;
    if (!showMenu(renderer)) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    Tank player(400, 500, 5 , renderer);
    std::vector<Bullet> bullets;
    std::vector<Bullet> enemyBullets;
    std::vector<Enemy> enemies;

    int playerLives = PLAYER_LIVES;
    // Load hình trái tim
SDL_Texture *heartTexture = loadTexture("C:/Users/ACER/Documents/Tap code/GameGem/assets/heart.png", renderer);

// Gọi hàm vẽ số mạng với hình trái tim
drawLives(renderer, heartTexture, playerLives);


    Uint32 lastEnemySpawnTime = SDL_GetTicks(); // Lưu thời điểm spawn enemy cuối cùng

while (running) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        player.move(e, obstacles);

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            bullets.emplace_back(player.getX() + TANK_SIZE / 2 - BULLET_SIZE / 2, player.getY(), 8, player.getDirection());
        }
    }

     // Kiểm tra nếu đủ 5 giây thì thêm Enemy mới (giảm từ 10s xuống 5s)
     if (SDL_GetTicks() - lastEnemySpawnTime >= 5000) {
        enemies.emplace_back(rand() % (SCREEN_WIDTH - ENEMY_SIZE), rand() % 200, 2, renderer);
        lastEnemySpawnTime = SDL_GetTicks(); // Cập nhật thời gian spawn mới
    }

    for (auto &bullet : bullets) {
        bullet.move();
    }

    for (auto &enemy : enemies) {
        enemy.move(obstacles);
        if (rand() % 100 < 2) {
            enemyBullets.emplace_back(enemy.getX() + ENEMY_SIZE / 2 - BULLET_SIZE / 2, enemy.getY(), 6, enemy.getDirection());
        }
    }

    for (auto &bullet : enemyBullets) {
        bullet.move();
    }

    for (auto &obs : obstacles) obs.draw(renderer);


    // Xóa đạn ra khỏi màn hình
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet &b) {
        return b.getX() < 0 || b.getX() > SCREEN_WIDTH || b.getY() < 0 || b.getY() > SCREEN_HEIGHT;
    }), bullets.end());

    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const Bullet &b) {
        return b.getX() < 0 || b.getX() > SCREEN_WIDTH || b.getY() < 0 || b.getY() > SCREEN_HEIGHT;
    }), enemyBullets.end());

    // Kiểm tra va chạm giữa đạn của người chơi và Enemy
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

// Kiểm tra va chạm giữa đạn của Enemy và người chơi
for (auto bullet = enemyBullets.begin(); bullet != enemyBullets.end();) {
    if (checkCollision(*bullet, player)) {
        bullet = enemyBullets.erase(bullet);
        playerLives--;

        if (playerLives <= 0) {
            showGameOver(renderer); // Hiển thị game over trong 5 giây
            running = false; // Kết thúc game
        }
        
    } else {
        ++bullet;
    }
}


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (auto &obs : obstacles) {
        obs.draw(renderer);
    }
    
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

    drawLives(renderer, heartTexture, playerLives);

    SDL_RenderPresent(renderer);

    SDL_Delay(16);
}

    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
