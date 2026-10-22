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

extern struct AnimClip *func_800448D8(struct AnimTrack *track, s32 index, s32 direction,
                                      u8 flags);

void func_80044988(u8 *data, struct AnimTrack *track, u8 forward, s32 index)
{
    if (data == 0) {
        track->clips = 0;
        return;
    }
    track->clips = (struct AnimClip *)(data + 4);
    track->count = data[0];
    func_800448D8(track, index, forward ? 1 : -1, 0);
    track->elapsed = 0;
    track->flags = 0;
}
