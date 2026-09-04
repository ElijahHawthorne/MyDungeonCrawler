#pragma once
#include "raylib.h"

struct Enemy {
    float x;
    float y;
    float speed;
    float size;
    float dirX;
    float dirY;
    float changeTimer;
    int health;
    int maxHealth;
    int attackPower;
    bool alive;
};

void UpdateEnemy(Enemy& enemy);
void DrawEnemy(const Enemy& enemy);