#include "Player.h"
#include "Map.h"
#include "Tile.h"

void UpdatePlayer(Player& player) {
    float delta = GetFrameTime();
    float newX = player.x;
    float newY = player.y;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) newX += player.speed * delta;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) newX -= player.speed * delta;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) newY += player.speed * delta;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) newY -= player.speed * delta;

    int tileScreenSize = TILE_SIZE * DRAW_SCALE;

    int colAtNewX = (int)(newX + player.size / 2) / tileScreenSize;
    int rowAtCurrentY = (int)(player.y + player.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtCurrentY, colAtNewX)) {
        player.x = newX;
    }

    int colAtCurrentX = (int)(player.x + player.size / 2) / tileScreenSize;
    int rowAtNewY = (int)(newY + player.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtNewY, colAtCurrentX)) {
        player.y = newY;
    }
}

void DrawPlayer(const Player& player) {
    DrawRectangle((int)player.x, (int)player.y, (int)player.size, (int)player.size, RED);
}