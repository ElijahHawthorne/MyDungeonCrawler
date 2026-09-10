#pragma once
#include "raylib.h"
#include "combat/Ability.h"

struct Player {
    float x;
    float y;
    float speed;
    float size;
    int health;
    int maxHealth;
    int energy;       // resource spent on abilities; regens 1 per combat round
    int maxEnergy;
    int attackPower;  // fallback melee power (used by enemies' turn logic pattern)
    Loadout loadout;  // abilities carried into combat
};

void UpdatePlayer(Player& player);
void DrawPlayer(const Player& player);