#include "Player.h"
#include "Map.h"
#include "Tile.h"

// Every frame in the spritesheet is this many pixels square, laid out in a
// single vertical strip (one column, frames stacked top to bottom) with no gaps.
static const int PLAYER_FRAME = 16;

// The one place spritesheet frame numbers live. Indexed by PlayerAnim.
// Frame ranges (0-based) for the current 19-frame sheet:
//   IDLE  frame 0
//   FLAME frames 1-4
//   HURT  frames 5-9
//   SLASH frames 10-13
//   MEND  frames 14-18
static const AnimClip PLAYER_CLIPS[PANIM_COUNT] = {
    /* PANIM_IDLE  */ { 0,  1, 1.0f,  true  },
    /* PANIM_FLAME */ { 1,  4, 12.0f, false },
    /* PANIM_HURT  */ { 5,  5, 12.0f, false },
    /* PANIM_SLASH */ { 10, 4, 12.0f, false },
    /* PANIM_MEND  */ { 14, 5, 12.0f, false },
};

void UpdatePlayer(Player& player) {
    float delta = GetFrameTime();
    float newX = player.x;
    float newY = player.y;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) newX += player.speed * delta;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) newX -= player.speed * delta;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) newY += player.speed * delta;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) newY -= player.speed * delta;

    int tileScreenSize = TILE_SIZE * DRAW_SCALE;

    int colAtNewX = (int)(newX + player.size / 2) / tileScreenSize;
    int rowAtCurrentY = (int)(player.y + player.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtCurrentY, colAtNewX)) {
        player.x = newX;
    }

    int colAtCurrentX = (int)(player.x + player.size / 2) / tileScreenSize;
    int rowAtNewY = (int)(newY + player.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtNewY, colAtCurrentX)) {
        player.y = newY;
    }
}

void SetPlayerAnim(Player& player, PlayerAnim anim) {
    player.anim = anim;
    player.animFrame = 0;
    player.animTimer = 0.0f;
}

void UpdatePlayerAnim(Player& player, float dt) {
    const AnimClip& clip = PLAYER_CLIPS[player.anim];
    if (clip.count <= 1) {
        player.animFrame = 0;
        return;
    }

    player.animTimer += dt;
    float frameTime = 1.0f / clip.fps;

    while (player.animTimer >= frameTime) {
        player.animTimer -= frameTime;
        player.animFrame++;

        if (player.animFrame >= clip.count) {
            if (clip.loop) {
                player.animFrame = 0;
            } else {
                SetPlayerAnim(player, PANIM_IDLE);
                return;
            }
        }
    }
}

float PlayerAnimDuration(PlayerAnim anim) {
    const AnimClip& clip = PLAYER_CLIPS[anim];
    return clip.count / clip.fps;
}

Rectangle PlayerFrameRect(const Player& player) {
    const AnimClip& clip = PLAYER_CLIPS[player.anim];
    int frame = clip.start + player.animFrame;
    return { 0.0f, (float)(frame * PLAYER_FRAME), (float)PLAYER_FRAME, (float)PLAYER_FRAME };
}

void DrawPlayer(const Player& player, Texture2D sheet) {
    if (sheet.id == 0) {
        DrawRectangle((int)player.x, (int)player.y, (int)player.size, (int)player.size, RED);
        return;
    }

    float drawSize = (float)(PLAYER_FRAME * DRAW_SCALE);
    Rectangle src = PlayerFrameRect(player);
    Rectangle dest = {
        player.x + player.size / 2.0f - drawSize / 2.0f, // centered on the hitbox
        player.y + player.size - drawSize,               // feet on the hitbox bottom
        drawSize,
        drawSize
    };
    DrawTexturePro(sheet, src, dest, { 0, 0 }, 0.0f, WHITE);
}
