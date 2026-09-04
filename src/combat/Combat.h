#pragma once
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "GameState.h"

bool CheckCollision(const Player& player, const Enemy& enemy);
void UpdateCombat(Player& player, Enemy& enemy, GameState& state);
void DrawCombat(const Player& player, const Enemy& enemy);