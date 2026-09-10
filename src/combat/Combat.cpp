#include "Combat.h"

static float enemyTurnTimer = 0.0f;
static bool waitingForEnemyTurn = false;
static int targetIndex = 0;
static int pendingAbility = 0; // index into player.loadout.slots

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
    pendingAbility = 0;
}

// Runs an ability's effect list. This is the only place effect behavior lives,
// so a new EffectType means adding exactly one case here.
static void ApplyAbility(const Ability& a, Player& player,
                         std::vector<Enemy*>& enemies, int target) {
    for (const Effect& e : a.effects) {
        switch (e.type) {
            case EffectType::Damage:
                if (a.target == TargetType::AllEnemies) {
                    for (Enemy* en : enemies)
                        if (en->alive) en->health -= e.amount;
                } else if (target >= 0 && target < (int)enemies.size()) {
                    enemies[target]->health -= e.amount;
                }
                break;
            case EffectType::Heal:
                player.health += e.amount;
                if (player.health > player.maxHealth) player.health = player.maxHealth;
                break;
        }
    }
}

static bool SlotUsable(const AbilitySlot& s, const Player& player) {
    return s.ability && s.cooldownRemaining == 0 && player.energy >= s.ability->cost;
}

// On-screen rectangle for each loadout slot. Shared by UpdateCombat (click
// detection) and DrawCombat (rendering) so the hitboxes always match.
static std::vector<Rectangle> GetLoadoutRects(const Loadout& loadout) {
    std::vector<Rectangle> rects;
    for (int i = 0; i < (int)loadout.slots.size() && i < 4; i++) {
        rects.push_back({ 20.0f + i * 195.0f, 338.0f, 185.0f, 100.0f });
    }
    return rects;
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
    Loadout& loadout = player.loadout;

    if (waitingForEnemyTurn) {
        enemyTurnTimer -= GetFrameTime();
        if (enemyTurnTimer <= 0) {
            for (Enemy* e : enemies) {
                if (e->alive) player.health -= e->attackPower;
            }
            waitingForEnemyTurn = false;

            // End of round: tick cooldowns and regenerate a point of energy.
            for (AbilitySlot& s : loadout.slots) {
                if (s.cooldownRemaining > 0) s.cooldownRemaining--;
            }
            if (player.energy < player.maxEnergy) player.energy++;

            if (player.health <= 0) {
                player.health = 0;
                state = GAME_OVER;
            }
        }
        return;
    }

    int slotCount = (int)loadout.slots.size();
    if (slotCount > 4) slotCount = 4;

    // Number keys 1-4 jump straight to a loadout slot.
    for (int i = 0; i < slotCount; i++) {
        if (IsKeyPressed(KEY_ONE + i)) pendingAbility = i;
    }

    // Left/right arrows cycle through the loadout.
    if (slotCount > 0) {
        if (IsKeyPressed(KEY_RIGHT)) pendingAbility = (pendingAbility + 1) % slotCount;
        if (IsKeyPressed(KEY_LEFT))  pendingAbility = (pendingAbility + slotCount - 1) % slotCount;
    }
    if (pendingAbility >= slotCount) pendingAbility = 0;

    // Mouse click: on a loadout slot selects that ability, on an enemy targets it.
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();

        std::vector<Rectangle> slotRects = GetLoadoutRects(loadout);
        for (int i = 0; i < (int)slotRects.size(); i++) {
            if (CheckCollisionPointRec(mousePos, slotRects[i])) pendingAbility = i;
        }

        std::vector<Rectangle> rects = GetEnemyRects(enemies);
        for (int i = 0; i < (int)enemies.size(); i++) {
            if (enemies[i]->alive && CheckCollisionPointRec(mousePos, rects[i])) {
                targetIndex = i;
                break;
            }
        }
    }

    // SPACE commits the selected ability against the current target.
    if (IsKeyPressed(KEY_SPACE) && !loadout.slots.empty()) {
        AbilitySlot& slot = loadout.slots[pendingAbility];
        if (SlotUsable(slot, player)) {
            const Ability& a = *slot.ability;

            ApplyAbility(a, player, enemies, targetIndex);
            player.energy -= a.cost;
            slot.cooldownRemaining = a.cooldown;

            for (Enemy* e : enemies) {
                if (e->alive && e->health <= 0) {
                    e->health = 0;
                    e->alive = false;
                }
            }

            if (!AnyEnemyAlive(enemies)) {
                state = EXPLORING;
                return;
            }

            waitingForEnemyTurn = true;
            enemyTurnTimer = 0.6f;
        }
    }
}

void DrawCombat(const Player& player, const std::vector<Enemy*>& enemies) {
    DrawRectangle(0, 0, 800, 450, Fade(BLACK, 0.7f));
    DrawText("Enemies appeared!", 260, 40, 20, WHITE);

    DrawText(TextFormat("Player HP: %d / %d", player.health, player.maxHealth), 100, 95, 20, WHITE);
    DrawText(TextFormat("Energy: %d / %d", player.energy, player.maxEnergy), 100, 118, 18, SKYBLUE);
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

    // --- Loadout bar ---
    const Loadout& loadout = player.loadout;
    DrawText("1-4 / arrows / click name: pick ability   |   click enemy: target   |   SPACE: use",
             90, 312, 15, WHITE);

    std::vector<Rectangle> slotRects = GetLoadoutRects(loadout);
    for (int i = 0; i < (int)slotRects.size(); i++) {
        const AbilitySlot& s = loadout.slots[i];
        if (!s.ability) continue;

        Rectangle box = slotRects[i];
        bool usable = s.cooldownRemaining == 0 && player.energy >= s.ability->cost;

        DrawRectangleRec(box, Fade(usable ? DARKGRAY : BLACK, 0.85f));
        if (i == pendingAbility) DrawRectangleLinesEx(box, 3, YELLOW);

        DrawText(TextFormat("[%d] %s", i + 1, s.ability->name.c_str()),
                 (int)box.x + 8, (int)box.y + 8, 18, usable ? WHITE : GRAY);

        if (s.cooldownRemaining > 0) {
            DrawText(TextFormat("Cooldown: %d", s.cooldownRemaining),
                     (int)box.x + 8, (int)box.y + 32, 15, ORANGE);
        } else if (s.ability->cost > 0) {
            DrawText(TextFormat("Cost: %d", s.ability->cost),
                     (int)box.x + 8, (int)box.y + 32, 15, SKYBLUE);
        }

        DrawText(s.ability->description.c_str(),
                 (int)box.x + 8, (int)box.y + 54, 12, LIGHTGRAY);
    }
}