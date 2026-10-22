#include "game/types.h"

struct ScriptTimer {
    s32 count;
    s32 period;
    s32 remaining;
    s32 trigger;
    s32 action;
};

extern struct ScriptTimer *D_8009F770;
extern s32 D_8009F774;

extern s32 func_80069D68(s32 id, s32 arg1, s32 arg2, s32 arg3);

void func_80049564(s32 elapsed)
{
    struct ScriptTimer *timer;
    s32 i;
    s32 remaining;

    timer = D_8009F770;
    for (i = 0; i < D_8009F774; i++, timer++) {
        if (timer->count == 0) {
            continue;
        }
        remaining = timer->remaining - elapsed;
        timer->remaining = remaining;
        if (remaining >= 0) {
            continue;
        }
        timer->remaining = remaining + timer->period;
        if (func_80069D68(timer->trigger, 0, -1, 0) != 0) {
            func_80069D68(timer->action, 0, -1, 0);
            if (timer->count > 0) {
                timer->count--;
            }
        }
    }
}
