#include "game/types.h"

struct ScriptTimer {
    s32 count;
    s32 period;
    s32 remaining;
    s32 trigger;
    s32 action;
};

extern struct ScriptTimer *D_8009F770;

void func_8004953C(s32 index, s32 kind, s32 value)
{
    struct ScriptTimer *timers;

    timers = D_8009F770;
    if (kind == 0x7E) {
        timers[index].count = value;
    }
}
