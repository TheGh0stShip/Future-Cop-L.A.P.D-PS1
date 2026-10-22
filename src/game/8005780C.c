#include "game/types.h"

struct InputAction {
    u32 buttons;
    s32 phase;
};

void func_8005780C(u32 buttons, struct InputAction *action)
{
    if (buttons & action->buttons) {
        if (action->phase == 2) {
            action->phase = 0;
        } else {
            action->phase = 3;
        }
    } else if (action->phase == 3) {
        action->phase = 1;
    } else if (action->phase != 2) {
        action->phase = 2;
    }
}
