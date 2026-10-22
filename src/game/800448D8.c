#include "game/types.h"

struct AnimClip {
    u8 unk00[4];
    s16 start;
    s16 end;
    u8 unk08[8];
};

struct AnimTrack {
    struct AnimClip *clips;
    struct AnimClip *clip;
    s32 elapsed;
    s16 frame;
    s16 target;
    s8 step;
    u8 index;
    u8 count;
    u8 flags;
};

struct AnimClip *func_800448D8(struct AnimTrack *track, s32 index, s32 direction, u8 flags)
{
    struct AnimClip *clip;

    if (track->clips == 0) {
        return 0;
    }
    track->index = index;
    clip = &track->clips[index];
    track->clip = clip;
    if (direction != 2) {
        track->elapsed = 0;
    }
    if (direction > 0) {
        track->step = 1;
        track->target = clip->end;
    } else {
        track->step = -1;
        track->target = clip->start;
    }
    if (clip->end < clip->start) {
        track->step = -track->step;
    }
    track->flags |= flags;
    track->frame = clip->start;
    return clip;
}
