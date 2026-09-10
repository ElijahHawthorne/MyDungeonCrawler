#pragma once

// One entry in a spritesheet animation. Frames are laid out in a single
// vertical strip (one column, stacked top to bottom, no gaps); `start` is the
// 0-based frame index of the clip's first frame.
struct AnimClip {
    int   start;
    int   count;
    float fps;
    bool  loop;
};
