#pragma once
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "GameState.h"
#include <vector>

bool CheckCollision(const Player& player, const Enemy& enemy);
void UpdateCombat(Player& player, std::vector<Enemy*>& enemies, GameState& state);
void DrawCombat(const Player& player, const std::vector<Enemy*>& enemies);
void ResetCombat();