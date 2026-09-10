#pragma once
#include "raylib.h"
#include "Anim.h"
#include "combat/Ability.h"

// Animation clips, in the same order as the blocks of the player spritesheet.
// Frame ranges live in PLAYER_CLIPS (Player.cpp).
enum PlayerAnim {
    PANIM_IDLE,   // also used for walking around and the battle stance
    PANIM_FLAME,  // fireball
    PANIM_HURT,   // taking damage
    PANIM_SLASH,
    PANIM_MEND,
    PANIM_COUNT
};

struct Player {
    float x;
    float y;
    float speed;
    float size;
    int health;
    int maxHealth;
    int energy;       // resource spent on abilities; regens 1 per combat round
    int maxEnergy;
    int attackPower;  // fallback melee power
    Loadout loadout;  // abilities carried into combat

    PlayerAnim anim;
    int        animFrame;  // current frame within the active clip (0-based)
    float      animTimer;
};

void UpdatePlayer(Player& player);

// Advances the active animation clip. Non-looping clips fall back to
// PANIM_IDLE once they finish.
void UpdatePlayerAnim(Player& player, float dt);

// Switches to a clip, restarting it from frame 0.
void SetPlayerAnim(Player& player, PlayerAnim anim);

// How long a clip takes to play through once, in seconds.
float PlayerAnimDuration(PlayerAnim anim);

// Source rectangle in the spritesheet for the player's current frame.
Rectangle PlayerFrameRect(const Player& player);

// Pass a spritesheet with id 0 (missing texture) to fall back to a plain rect.
void DrawPlayer(const Player& player, Texture2D sheet);
