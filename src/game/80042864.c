#include "game/types.h"

struct Unk42864 {
    u8 unk00[0x15];
    u8 total;
    u8 first;
    u8 second;
};

void func_80042864(struct Unk42864 *object, s32 amount)
{
    if (object->total == 0) {
        return;
    }
    if (object->second != 0) {
        if (object->second >= amount) {
            object->second -= amount;
            return;
        }
        amount -= object->second;
        object->second = 0;
    }
    if (object->first != 0) {
        if (object->first >= amount) {
            object->first -= amount;
            return;
        }
        amount -= object->first;
        object->first = 0;
    }
    if (object->total != 0) {
        if (object->total < amount) {
            object->total = 0;
        } else {
            object->total -= amount;
        }
    }
}
