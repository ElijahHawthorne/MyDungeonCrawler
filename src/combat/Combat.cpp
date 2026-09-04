#include "Combat.h"

static float enemyTurnTimer = 0.0f;
static bool waitingForEnemyTurn = false;
static int targetIndex = 0;

bool CheckCollision(const Player& player, const Enemy& enemy) {
    if (!enemy.alive) return false;

    return player.x < enemy.x + enemy.size &&
           player.x + player.size > enemy.x &&
           player.y < enemy.y + enemy.size &&
           player.y + player.size > enemy.y;
}

void ResetCombat() {
    waitingForEnemyTurn = false;
    enemyTurnTimer = 0.0f;
    targetIndex = 0;
}

bool AnyEnemyAlive(std::vector<Enemy*>& enemies) {
    for (Enemy* e : enemies) {
        if (e->alive) return true;
    }
    return false;
}

// Computes the on-screen rectangle for each enemy in the combat display.
// Both UpdateCombat (for click detection) and DrawCombat (for rendering)
// call this, so positions always match exactly.
std::vector<Rectangle> GetEnemyRects(const std::vector<Enemy*>& enemies) {
    std::vector<Rectangle> rects;
    int xOffset = 400;
    for (size_t i = 0; i < enemies.size(); i++) {
        rects.push_back({ (float)xOffset, 150, 150, 150 });
        xOffset += 180;
    }
    return rects;
}

void EnsureValidTarget(std::vector<Enemy*>& enemies) {
    if (targetIndex >= 0 && targetIndex < (int)enemies.size() && enemies[targetIndex]->alive) {
        return; // current target is still valid
    }
    for (int i = 0; i < (int)enemies.size(); i++) {
        if (enemies[i]->alive) {
            targetIndex = i;
            return;
        }
    }
}

void UpdateCombat(Player& player, std::vector<Enemy*>& enemies, GameState& state) {
    EnsureValidTarget(enemies);

    if (waitingForEnemyTurn) {
        enemyTurnTimer -= GetFrameTime();
        if (enemyTurnTimer <= 0) {
            for (Enemy* e : enemies) {
                if (e->alive) player.health -= e->attackPower;
            }
            waitingForEnemyTurn = false;

            if (player.health <= 0) {
                player.health = 0;
                state = GAME_OVER;
            }
        }
        return;
    }

    // Mouse click selects a target
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        std::vector<Rectangle> rects = GetEnemyRects(enemies);
        Vector2 mousePos = GetMousePosition();

        for (int i = 0; i < (int)enemies.size(); i++) {
            if (enemies[i]->alive && CheckCollisionPointRec(mousePos, rects[i])) {
                targetIndex = i;
                break;
            }
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        Enemy* target = enemies[targetIndex];

        target->health -= player.attackPower;
        if (target->health <= 0) {
            target->health = 0;
            target->alive = false;
        }

        if (!AnyEnemyAlive(enemies)) {
            state = EXPLORING;
            return;
        }

        waitingForEnemyTurn = true;
        enemyTurnTimer = 0.6f;
    }
}

void DrawCombat(const Player& player, const std::vector<Enemy*>& enemies) {
    DrawRectangle(0, 0, 800, 450, Fade(BLACK, 0.7f));
    DrawText("Enemies appeared!", 260, 40, 20, WHITE);

    DrawText(TextFormat("Player HP: %d / %d", player.health, player.maxHealth), 100, 100, 20, WHITE);
    DrawRectangle(80, 150, 150, 150, RED);

    std::vector<Rectangle> rects = GetEnemyRects(enemies);

    for (int i = 0; i < (int)enemies.size(); i++) {
        if (!enemies[i]->alive) continue;

        DrawText(TextFormat("Enemy HP: %d / %d", enemies[i]->health, enemies[i]->maxHealth),
                  (int)rects[i].x, 100, 18, WHITE);
        DrawRectangleRec(rects[i], PURPLE);

        if (i == targetIndex) {
            DrawRectangleLinesEx(rects[i], 4, YELLOW); // highlight the targeted enemy
        }
    }

    DrawText("Click an enemy to target, SPACE to attack", 210, 380, 20, WHITE);
}