#include "Enemy.h"
#include "Map.h"
#include "Tile.h"

// Every frame in the spritesheet is this many pixels square, laid out in a
// single vertical strip (frames stacked top to bottom) with no gaps.
static const int ENEMY_FRAME = 16;

// The one place spritesheet frame numbers live. Indexed by EnemyAnim.
// Frame ranges (0-based) for the current 15-frame sheet:
//   IDLE        frame 0
//   ATTACK      frames 1-4
//   HURT_SLASH  frames 6-9
//   HURT_FIRE   frames 11-14
static const AnimClip ENEMY_CLIPS[EANIM_COUNT] = {
    /* EANIM_IDLE       */ { 0,  1, 1.0f,  true  },
    /* EANIM_ATTACK     */ { 1,  4, 12.0f, false },
    /* EANIM_HURT_SLASH */ { 6,  4, 12.0f, false },
    /* EANIM_HURT_FIRE  */ { 11, 4, 12.0f, false },
};

void PickNewDirection(Enemy& enemy) {
    // 0=up, 1=down, 2=left, 3=right, 4=stand still
    int choice = GetRandomValue(0, 4);
    switch (choice) {
        case 0: enemy.dirX = 0;  enemy.dirY = -1; break;
        case 1: enemy.dirX = 0;  enemy.dirY = 1;  break;
        case 2: enemy.dirX = -1; enemy.dirY = 0;  break;
        case 3: enemy.dirX = 1;  enemy.dirY = 0;  break;
        default: enemy.dirX = 0; enemy.dirY = 0;  break;
    }
    enemy.changeTimer = (float)GetRandomValue(1, 3); // wander 1-3 seconds before changing again
}

void UpdateEnemy(Enemy& enemy) {
    float delta = GetFrameTime();

    enemy.changeTimer -= delta;
    if (enemy.changeTimer <= 0) {
        PickNewDirection(enemy);
    }

    float newX = enemy.x + enemy.dirX * enemy.speed * delta;
    float newY = enemy.y + enemy.dirY * enemy.speed * delta;

    int tileScreenSize = TILE_SIZE * DRAW_SCALE;

    int colAtNewX = (int)(newX + enemy.size / 2) / tileScreenSize;
    int rowAtCurrentY = (int)(enemy.y + enemy.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtCurrentY, colAtNewX)) {
        enemy.x = newX;
    } else {
        PickNewDirection(enemy); // hit something, try a new direction immediately
    }

    int colAtCurrentX = (int)(enemy.x + enemy.size / 2) / tileScreenSize;
    int rowAtNewY = (int)(newY + enemy.size / 2) / tileScreenSize;
    if (IsWalkable(rowAtNewY, colAtCurrentX)) {
        enemy.y = newY;
    } else {
        PickNewDirection(enemy);
    }
}

void SetEnemyAnim(Enemy& enemy, EnemyAnim anim) {
    enemy.anim = anim;
    enemy.animFrame = 0;
    enemy.animTimer = 0.0f;
}

void UpdateEnemyAnim(Enemy& enemy, float dt) {
    const AnimClip& clip = ENEMY_CLIPS[enemy.anim];
    if (clip.count <= 1) {
        enemy.animFrame = 0;
        return;
    }

    enemy.animTimer += dt;
    float frameTime = 1.0f / clip.fps;

    while (enemy.animTimer >= frameTime) {
        enemy.animTimer -= frameTime;
        enemy.animFrame++;

        if (enemy.animFrame >= clip.count) {
            if (clip.loop) {
                enemy.animFrame = 0;
            } else {
                SetEnemyAnim(enemy, EANIM_IDLE);
                return;
            }
        }
    }
}

float EnemyAnimDuration(EnemyAnim anim) {
    const AnimClip& clip = ENEMY_CLIPS[anim];
    return clip.count / clip.fps;
}

Rectangle EnemyFrameRect(const Enemy& enemy) {
    const AnimClip& clip = ENEMY_CLIPS[enemy.anim];
    int frame = clip.start + enemy.animFrame;
    return { 0.0f, (float)(frame * ENEMY_FRAME), (float)ENEMY_FRAME, (float)ENEMY_FRAME };
}

void DrawEnemy(const Enemy& enemy, Texture2D sheet) {
    if (sheet.id == 0) {
        DrawRectangle((int)enemy.x, (int)enemy.y, (int)enemy.size, (int)enemy.size, PURPLE);
        return;
    }

    float drawSize = (float)(ENEMY_FRAME * DRAW_SCALE);
    Rectangle src = EnemyFrameRect(enemy);
    Rectangle dest = {
        enemy.x + enemy.size / 2.0f - drawSize / 2.0f, // centered on the hitbox
        enemy.y + enemy.size - drawSize,               // feet on the hitbox bottom
        drawSize,
        drawSize
    };
    DrawTexturePro(sheet, src, dest, { 0, 0 }, 0.0f, WHITE);
}
