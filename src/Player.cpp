#include "Player.h"

void UpdatePlayer(Player& player) {
    float delta = GetFrameTime();
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.x += player.speed * delta;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) player.x -= player.speed * delta;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) player.y += player.speed * delta;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) player.y -= player.speed * delta;
}

void DrawPlayer(const Player& player) {
    DrawRectangle((int)player.x, (int)player.y, (int)player.size, (int)player.size, RED);
}