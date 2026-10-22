#include "game/types.h"

extern void func_8001846C(u32 id, s32 *first, s32 *second);

s32 func_8006062C(u32 id)
{
    s32 first;
    s32 second;

    if (id >= 0x800) {
        return -0x10000;
    }
    func_8001846C(id, &first, &second);
    return second;
}
