#include "raylib.h"
#include "Tile.h"
#include "Map.h"
#include "entities/Player.h"
#include "entities/Enemy.h"
#include "GameState.h"
#include "combat/Combat.h"
#include "combat/Ability.h"
#include <vector>

int main() {
    InitWindow(800, 450, "Grass Field");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D tileset = LoadTexture("assets/tilesets/tilemap.png");
    Texture2D playerSheet = LoadTexture("assets/sprites/player.png"); // id 0 if missing -> rect fallback
    Texture2D enemySheet  = LoadTexture("assets/sprites/enemy.png");
    LoadMapFromFile("assets/maps/field.json");

    AbilityDatabase abilities;
    abilities.LoadFromFile("assets/data/abilities.json");

    Music exploreMusic = LoadMusicStream("assets/music/exploring.mp3");
    Music combatMusic  = LoadMusicStream("assets/music/combat.mp3");
    exploreMusic.looping = true;
    combatMusic.looping = true;

    Sound recordScratch = LoadSound("assets/sounds/recordScratch.mp3");
    SetSoundPitch(recordScratch, 1.3f);

    int tileScreenSize = TILE_SIZE * DRAW_SCALE;
    float startX = (float)(playerStartCol * tileScreenSize);
    float startY = (float)(playerStartRow * tileScreenSize);

    Player player = { startX, startY, 200.0f, (float)(TILE_SIZE * DRAW_SCALE / 2),
                      30, 30, 5, 5, 15, {}, PANIM_IDLE, 0, 0.0f };
    player.loadout = BuildLoadout(abilities, { "slash", "fireball", "mend" });

    std::vector<Enemy> mapEnemies = {
        { (float)(10 * tileScreenSize), (float)(7 * tileScreenSize), 80.0f, (float)(TILE_SIZE * DRAW_SCALE / 2), 0, 0, 0, 20, 20, 3, true },
        { (float)(12 * tileScreenSize), (float)(8 * tileScreenSize), 80.0f, (float)(TILE_SIZE * DRAW_SCALE / 2), 0, 0, 0, 20, 20, 3, true }
    };

    Enemy hiddenPartner = { 0, 0, 0, 0, 0, 0, 0, 15, 15, 2, true };
    const int groupEnemyIndex = 0;

    GameState state = EXPLORING;
    GameState previousState = EXPLORING;
    std::vector<Enemy*> activeCombatEnemies;

    const float MUSIC_FADE_DURATION = 1.0f;
    bool fadingOutCombat = false;
    float fadeTimer = 0.0f;
    bool waitingToStartCombat = false;

    PlayMusicStream(exploreMusic);

    while (!WindowShouldClose()) {
        if (state == EXPLORING) {
            UpdatePlayer(player);

            for (int i = 0; i < (int)mapEnemies.size(); i++) {
                if (!mapEnemies[i].alive) continue;

                UpdateEnemy(mapEnemies[i]);

                if (CheckCollision(player, mapEnemies[i])) {
                    activeCombatEnemies.clear();
                    activeCombatEnemies.push_back(&mapEnemies[i]);

                    if (i == groupEnemyIndex) {
                        hiddenPartner.alive = true;
                        activeCombatEnemies.push_back(&hiddenPartner);
                    }

                    ResetCombat();
                    state = COMBAT;
                    break;
                }
            }
        } else if (state == COMBAT) {
            UpdateCombat(player, activeCombatEnemies, state);
        }

        UpdatePlayerAnim(player, GetFrameTime());
        for (Enemy& e : mapEnemies) UpdateEnemyAnim(e, GetFrameTime());
        UpdateEnemyAnim(hiddenPartner, GetFrameTime());

        // --- Music/sound transitions ---
        if (state != previousState) {
            if (state == COMBAT) {
                StopMusicStream(exploreMusic);
                PlaySound(recordScratch); // scratch plays ONLY when entering combat
                waitingToStartCombat = true;
            } else if (state == EXPLORING) {
                fadingOutCombat = true;
                fadeTimer = MUSIC_FADE_DURATION;
            }
            previousState = state;
        }

        // --- Wait for the scratch to actually finish before starting combat music ---
        if (waitingToStartCombat && !IsSoundPlaying(recordScratch)) {
            waitingToStartCombat = false;
            SetMusicVolume(combatMusic, 1.0f);
            PlayMusicStream(combatMusic);
        }

        // --- Fade combat music out, then start exploring music ---
        if (fadingOutCombat) {
            fadeTimer -= GetFrameTime();
            float volume = fadeTimer / MUSIC_FADE_DURATION;
            if (volume < 0.0f) volume = 0.0f;
            SetMusicVolume(combatMusic, volume);
            UpdateMusicStream(combatMusic);

            if (fadeTimer <= 0.0f) {
                StopMusicStream(combatMusic);
                fadingOutCombat = false;
                PlayMusicStream(exploreMusic);
            }
        } else {
            if (state == EXPLORING && !waitingToStartCombat) UpdateMusicStream(exploreMusic);
            if (state == COMBAT && !waitingToStartCombat) UpdateMusicStream(combatMusic);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawMap(tileset);
        DrawPlayer(player, playerSheet);

        for (const Enemy& e : mapEnemies) {
            if (e.alive) DrawEnemy(e, enemySheet);
        }

        if (state == COMBAT) {
            DrawCombat(player, activeCombatEnemies, playerSheet, enemySheet);
        } else if (state == GAME_OVER) {
            DrawText("GAME OVER", 300, 200, 30, RED);
        }

        EndDrawing();
    }

    UnloadSound(recordScratch);
    UnloadMusicStream(exploreMusic);
    UnloadMusicStream(combatMusic);
    UnloadTexture(tileset);
    UnloadTexture(playerSheet);
    UnloadTexture(enemySheet);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}