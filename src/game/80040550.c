#include "game/types.h"

struct Unk40550Timers {
    u8 unk00[9];
    u8 total;
    u8 first;
    u8 second;
};

struct Unk40550 {
    u8 unk00[0x6C];
    struct Unk40550Timers *timers;
};

void func_80040550(struct Unk40550 *object, s32 amount)
{
    if (object->timers->total == 0) {
        return;
    }
    if (object->timers->second != 0) {
        if (object->timers->second >= amount) {
            object->timers->second -= amount;
            return;
        }
        amount -= object->timers->second;
        object->timers->second = 0;
    }
    if (object->timers->first != 0) {
        if (object->timers->first >= amount) {
            object->timers->first -= amount;
            return;
        }
        amount -= object->timers->first;
        object->timers->first = 0;
    }
    if (object->timers->total != 0) {
        if (object->timers->total < amount) {
            object->timers->total = 0;
        } else {
            object->timers->total -= amount;
        }
    }
}
