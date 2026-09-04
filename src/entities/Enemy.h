#pragma once
#include "raylib.h"

struct Enemy {
    float x;
    float y;
    float speed;
    float size;
    float dirX;       // current movement direction: -1, 0, or 1
    float dirY;
    float changeTimer; // seconds until picking a new random direction
};

void UpdateEnemy(Enemy& enemy);
void DrawEnemy(const Enemy& enemy);