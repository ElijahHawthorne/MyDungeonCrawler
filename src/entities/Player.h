#pragma once
#include "raylib.h"

struct Player {
    float x;
    float y;
    float speed;
    float size;
    int health;
    int maxHealth;
    int attackPower;
};

void UpdatePlayer(Player& player);
void DrawPlayer(const Player& player);