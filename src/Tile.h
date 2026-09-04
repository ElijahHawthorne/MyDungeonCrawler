#pragma once
#include "raylib.h"

const int TILE_SIZE = 16;
const int TILE_STRIDE = 17;
const int DRAW_SCALE = 2;

const int GRASS = 0;
const int TRAIL = 1;
const int TREE  = 2;

extern const Rectangle TILE_GRASS_SRC;
extern const Rectangle TILE_GRASS_TEXTURED_SRC;
extern const Rectangle TILE_GRASS_FLOWER_SRC;
extern const Rectangle TILE_TREE_SRC;
extern const Rectangle TILE_TRAIL_SRC;

Rectangle GetTileSource(int tileType);