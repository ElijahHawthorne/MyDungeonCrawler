#pragma once
#include "raylib.h"
#include <unordered_set>
#include <string>

const int MAP_COLS = 15;
const int MAP_ROWS = 10;

extern int map[MAP_ROWS][MAP_COLS];
extern const std::unordered_set<int> BLOCKED_TILES;

void DrawMap(Texture2D tileset);
bool IsWalkable(int row, int col);
void LoadMapFromFile(const std::string& path);