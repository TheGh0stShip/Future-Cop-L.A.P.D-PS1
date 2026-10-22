#include "game/types.h"

extern void func_80021A70(u32 mask);

void func_800221E4(s32 count, s32 *channels)
{
    u32 mask;
    s32 i;

    mask = 0;
    for (i = 0; i < count; i++, channels++) {
        if (*channels != -1) {
            mask |= 1 << *channels;
        }
    }
    func_80021A70(mask);
}
