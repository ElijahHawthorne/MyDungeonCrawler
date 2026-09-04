#include "Map.h"
#include "Tile.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;


int map[MAP_ROWS][MAP_COLS] = {};

const std::unordered_set<int> BLOCKED_TILES = { TREE };


void LoadMapFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERROR: Could not open map file: " << path << std::endl;
        return;
    }

    json data = json::parse(file);

    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            map[row][col] = data["tiles"][row][col];
        }
    }
}

void DrawMap(Texture2D tileset) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            Rectangle dest = {
                (float)(col * TILE_SIZE * DRAW_SCALE),
                (float)(row * TILE_SIZE * DRAW_SCALE),
                (float)(TILE_SIZE * DRAW_SCALE),
                (float)(TILE_SIZE * DRAW_SCALE)
            };

            DrawTexturePro(tileset, TILE_GRASS_SRC, dest, {0, 0}, 0.0f, WHITE);

            int tileType = map[row][col];
            if (tileType != GRASS) {
                Rectangle source = GetTileSource(tileType);
                DrawTexturePro(tileset, source, dest, {0, 0}, 0.0f, WHITE);
            }
        }
    }
}

bool IsWalkable(int row, int col) {
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS) {
        return false;
    }
    return BLOCKED_TILES.find(map[row][col]) == BLOCKED_TILES.end();
}