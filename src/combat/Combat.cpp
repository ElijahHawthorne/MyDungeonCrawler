#include "Combat.h"
#include <string>

static float enemyTurnTimer = 0.0f;
static bool waitingForEnemyTurn = false;

bool CheckCollision(const Player& player, const Enemy& enemy) {
    if (!enemy.alive) return false;

    return player.x < enemy.x + enemy.size &&
           player.x + player.size > enemy.x &&
           player.y < enemy.y + enemy.size &&
           player.y + player.size > enemy.y;
}

void UpdateCombat(Player& player, Enemy& enemy, GameState& state) {
    if (waitingForEnemyTurn) {
        enemyTurnTimer -= GetFrameTime();
        if (enemyTurnTimer <= 0) {
            player.health -= enemy.attackPower;
            waitingForEnemyTurn = false;

            if (player.health <= 0) {
                player.health = 0;
                state = GAME_OVER;
            }
        }
        return; // don't accept input while waiting for enemy's turn
    }

    if (IsKeyPressed(KEY_SPACE)) {
        enemy.health -= player.attackPower;

        if (enemy.health <= 0) {
            enemy.health = 0;
            enemy.alive = false;
            state = EXPLORING; // won the fight
            return;
        }

        // queue the enemy's counter-attack after a short delay
        waitingForEnemyTurn = true;
        enemyTurnTimer = 0.6f;
    }
}

void DrawCombat(const Player& player, const Enemy& enemy) {
    DrawRectangle(0, 0, 800, 450, Fade(BLACK, 0.7f));

    DrawText("A wild enemy appeared!", 220, 60, 20, WHITE);

    DrawText(TextFormat("Player HP: %d / %d", player.health, player.maxHealth), 100, 150, 20, WHITE);
    DrawText(TextFormat("Enemy HP: %d / %d", enemy.health, enemy.maxHealth), 500, 150, 20, WHITE);

    DrawRectangle(80, 200, 200, 200, RED);
    DrawRectangle(500, 200, 200, 200, PURPLE);

    DrawText("Press SPACE to attack", 260, 380, 20, WHITE);
}