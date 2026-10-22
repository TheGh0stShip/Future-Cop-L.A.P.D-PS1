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
    register struct AnimTrack *state __asm__("$8");
    struct AnimClip *clip;
    register struct AnimClip *computed __asm__("$2");
    register s32 offset __asm__("$3");
    register u16 frame __asm__("$4");
    u8 mergedFlags;

    state = track;
    __asm__ volatile ("" : "+r" (state));
    offset = index << 4;
    if (state->clips == 0) {
        computed = 0;
        goto done;
    }
    state->index = index;
    computed = (struct AnimClip *)((u8 *)state->clips + offset);
    clip = computed;
    state->clip = computed;
    if (direction != 2) {
        state->elapsed = 0;
    }
    if (direction > 0) {
        state->step = 1;
        state->target = clip->end;
    } else {
        state->step = -1;
        state->target = clip->start;
    }
    if (clip->start > clip->end) {
        state->step = -state->step;
    }
    computed = clip;
    mergedFlags = state->flags | flags;
    frame = clip->start;
    state->frame = frame;
    state->flags = mergedFlags;
done:
    return computed;
}
