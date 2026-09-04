#include "raylib.h"
#include "Tile.h"
#include "Map.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "GameState.h"
#include "combat/Combat.h"

int main() {
    InitWindow(800, 450, "Grass Field");
    SetTargetFPS(60);

    Texture2D tileset = LoadTexture("assets/tilesets/tilemap.png");
    LoadMapFromFile("assets/maps/field.json");

    int tileScreenSize = TILE_SIZE * DRAW_SCALE;
    float startX = (float)(playerStartCol * tileScreenSize);
    float startY = (float)(playerStartRow * tileScreenSize);

    Player player = { startX, startY, 200.0f, (float)(TILE_SIZE * DRAW_SCALE / 2), 30, 30, 5 };
    Enemy enemy = { (float)( 11 * tileScreenSize), (float)( 7 * tileScreenSize), 80.0f, (float)(TILE_SIZE * DRAW_SCALE / 2), 0, 0, 0, 20, 20, 3, true };

    GameState state = EXPLORING;



    while (!WindowShouldClose()) {
        if (state == EXPLORING) {
            UpdatePlayer(player);
            if (enemy.alive) {
                UpdateEnemy(enemy);
                if (CheckCollision(player, enemy)) {
                    state = COMBAT;
                }
            }
        } else if (state == COMBAT) {
            UpdateCombat(player, enemy, state);
        }
        // GAME_OVER: no updates, frozen

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMap(tileset);
        DrawPlayer(player);
        if (enemy.alive) DrawEnemy(enemy);

        if (state == COMBAT) {
            DrawCombat(player, enemy);
        } else if (state == GAME_OVER) {
            DrawText("GAME OVER", 300, 200, 30, RED);
        }    

        EndDrawing();
    }

    UnloadTexture(tileset);
    CloseWindow();
    return 0;
}