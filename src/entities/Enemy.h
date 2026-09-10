#pragma once
#include "raylib.h"
#include "Anim.h"

// Animation clips, in the same order as the blocks of the enemy spritesheet.
// Frame ranges live in ENEMY_CLIPS (Enemy.cpp).
enum EnemyAnim {
    EANIM_IDLE,
    EANIM_ATTACK,
    EANIM_HURT_SLASH,  // struck by a slash ability
    EANIM_HURT_FIRE,   // struck by a fire ability
    EANIM_COUNT
};

struct Enemy {
    float x;
    float y;
    float speed;
    float size;
    float dirX;
    float dirY;
    float changeTimer;
    int health;
    int maxHealth;
    int attackPower;
    bool alive;

    EnemyAnim anim;
    int       animFrame;
    float     animTimer;
};

void UpdateEnemy(Enemy& enemy);

// Advances the active animation clip; non-looping clips fall back to EANIM_IDLE.
void UpdateEnemyAnim(Enemy& enemy, float dt);

// Switches to a clip, restarting it from frame 0.
void SetEnemyAnim(Enemy& enemy, EnemyAnim anim);

// How long a clip takes to play through once, in seconds.
float EnemyAnimDuration(EnemyAnim anim);

// Source rectangle in the spritesheet for the enemy's current frame.
Rectangle EnemyFrameRect(const Enemy& enemy);

// Pass a spritesheet with id 0 (missing texture) to fall back to a plain rect.
void DrawEnemy(const Enemy& enemy, Texture2D sheet);
