#include "Tile.h"

const Rectangle TILE_GRASS_SRC          = { 0 * (float)TILE_STRIDE, 0 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_GRASS_TEXTURED_SRC = { 1 * (float)TILE_STRIDE, 0 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_GRASS_FLOWER_SRC   = { 2 * (float)TILE_STRIDE, 0 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_TREE_SRC           = { 4 * (float)TILE_STRIDE, 2 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_TRAIL_SRC          = { 1 * (float)TILE_STRIDE, 2 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };

Rectangle GetTileSource(int tileType) {
    switch (tileType) {
        case GRASS: return TILE_GRASS_SRC;
        case TRAIL: return TILE_TRAIL_SRC;
        case TREE:  return TILE_TREE_SRC;
        default:    return TILE_GRASS_SRC;
    }
}