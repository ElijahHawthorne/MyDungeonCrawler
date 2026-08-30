// main.cpp
#include "raylib.h"

const int TILE_SIZE = 16;      // actual tile width/height
const int TILE_STRIDE = 17;    // distance between tile origins (16 + 1px gap)
const int DRAW_SCALE = 2;
const int MAP_COLS = 15;
const int MAP_ROWS = 10;

const int GRASS = 0;
const int TRAIL = 1;
const int TREE  = 2;

const Rectangle TILE_GRASS_SRC          = { 0 * (float)TILE_STRIDE, 0 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_GRASS_TEXTURED_SRC = { 1 * (float)TILE_STRIDE, 0 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_GRASS_FLOWER_SRC   = { 2 * (float)TILE_STRIDE, 0 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_TREE_SRC      = { 4 * (float)TILE_STRIDE, 2 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };
const Rectangle TILE_TRAIL_SRC    = { 1 * (float)TILE_STRIDE, 2 * (float)TILE_STRIDE, (float)TILE_SIZE, (float)TILE_SIZE };

int map[MAP_ROWS][MAP_COLS] = {
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS },
    { TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE  },
    { GRASS, GRASS, TRAIL, TRAIL, TRAIL, TRAIL, TRAIL, TRAIL, TRAIL, TRAIL, TRAIL, TRAIL, TRAIL, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TRAIL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TRAIL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, TRAIL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE  },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TRAIL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TRAIL, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS }
};

Rectangle GetTileSource(int tileType) {
    switch (tileType) {
        case GRASS: return TILE_GRASS_SRC;
        case TRAIL: return TILE_TRAIL_SRC;
        case TREE:  return TILE_TREE_SRC;
        default:    return TILE_GRASS_SRC;
    }
}

int main() {
    InitWindow(800, 450, "Grass Field");
    SetTargetFPS(60);

    Texture2D tileset = LoadTexture("assets/tilesets/tilemap.png");

    // Player state
    float playerX = 100.0f;
    float playerY = 100.0f;
    float playerSpeed = 100.0f; // pixels per second
    float playerSize = (float)(TILE_SIZE * DRAW_SCALE/2); // same size as one tile

    while (!WindowShouldClose()) {
        // --- Input / movement ---
        float delta = GetFrameTime();
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) playerX += playerSpeed * delta;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) playerX -= playerSpeed * delta;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) playerY += playerSpeed * delta;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) playerY -= playerSpeed * delta;

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

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

        // Draw player on top of everything
        DrawRectangle((int)playerX, (int)playerY, (int)playerSize, (int)playerSize, RED);

        EndDrawing();
    }

    UnloadTexture(tileset);
    CloseWindow();
    return 0;
}
    