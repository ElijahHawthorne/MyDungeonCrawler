#include "Enemy.h"
#include "Map.h"
#include "Tile.h"

void PickNewDirection(Enemy& enemy) {
    // 0=up, 1=down, 2=left, 3=right, 4=stand still
    int choice = GetRandomValue(0, 4);
    switch (choice) {
        case 0: enemy.dirX = 0;  enemy.dirY = -1; break;
        case 1: enemy.dirX = 0;  enemy.dirY = 1;  break;
        case 2: enemy.dirX = -1; enemy.dirY = 0;  break;
        case 3: enemy.dirX = 1;  enemy.dirY = 0;  break;
        default: enemy.dirX = 0; enemy.dirY = 0;  break;
    }
    enemy.changeTimer = (float)GetRandomValue(1, 3); // wander 1-3 seconds before changing again
}

void UpdateEnemy(Enemy& enemy) {
    float delta = GetFrameTime();

    enemy.changeTimer -= delta;
    if (enemy.changeTimer <= 0) {
        PickNewDirection(enemy);
    }

    float newX = enemy.x + enemy.dirX * enemy.speed * delta;
    float newY = enemy.y + enemy.dirY * enemy.speed * delta;

    int tileScreenSize = TILE_SIZE * DRAW_SCALE;

    int colAtNewX = (int)(newX + enemy.size / 2) / tileScreenSize;
    int rowAtCurrentY = (int)(enemy.y + enemy.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtCurrentY, colAtNewX)) {
        enemy.x = newX;
    } else {
        PickNewDirection(enemy); // hit something, try a new direction immediately
    }

    int colAtCurrentX = (int)(enemy.x + enemy.size / 2) / tileScreenSize;
    int rowAtNewY = (int)(newY + enemy.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtNewY, colAtCurrentX)) {
        enemy.y = newY;
    } else {
        PickNewDirection(enemy);
    }
}

void DrawEnemy(const Enemy& enemy) {
    DrawRectangle((int)enemy.x, (int)enemy.y, (int)enemy.size, (int)enemy.size, PURPLE);
}