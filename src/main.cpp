#include "raylib.h"
#include "Tile.h"
#include "Map.h"
#include "Player.h"

int main() {
    InitWindow(800, 450, "Grass Field");
    SetTargetFPS(60);

    Texture2D tileset = LoadTexture("assets/tilesets/tilemap.png");
    LoadMapFromFile("assets/maps/field.json");

    Player player = { 100.0f, 100.0f, 200.0f, (float)(TILE_SIZE * DRAW_SCALE / 2) };

    while (!WindowShouldClose()) {
        UpdatePlayer(player);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMap(tileset);
        DrawPlayer(player);

        EndDrawing();
    }

    UnloadTexture(tileset);
    CloseWindow();
    return 0;
}