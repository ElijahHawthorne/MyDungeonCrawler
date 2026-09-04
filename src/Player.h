#pragma once
#include "raylib.h"

struct Player {
    float x;
    float y;
    float speed;
    float size;
};

void UpdatePlayer(Player& player);
void DrawPlayer(const Player& player);