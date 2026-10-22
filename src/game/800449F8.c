#include "game/types.h"

struct AnimClip;

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

struct AnimCommand {
    u8 op;
    u8 arg;
};

struct AnimScript {
    u8 unk00[0x0C];
    struct AnimCommand commands[4];
};

extern struct AnimClip *func_800448D8(struct AnimTrack *track, s32 index, s32 direction,
                                      u8 flags);
extern void func_8002A5B4(struct AnimScript *script, s32 index);

void func_800449F8(struct AnimScript *script, struct AnimTrack *track)
{
    s32 i;
    s32 op;
    s32 arg;

    for (i = 0; i < 4;) {
        op = script->commands[i].op;
        if (op == 0) {
            break;
        }
        arg = script->commands[i].arg;
        switch (op) {
        case 0x32:
            func_800448D8(track, arg, 1, 0);
            break;
        case 0x33:
            track->flags &= ~2;
            if (arg >= 0) {
                track->frame = arg;
            }
            break;
        case 0x34:
            track->flags |= 2;
            if (arg >= 0) {
                track->frame = arg;
            }
            break;
        default:
            i++;
            continue;
        }
        func_8002A5B4(script, i);
    }
}
