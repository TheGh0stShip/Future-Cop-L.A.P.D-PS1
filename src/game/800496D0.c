#include "game/types.h"

extern void **D_8009F780;
extern s32 D_8009F78C;
extern s32 D_8009F790;

extern void *func_80017EBC(s32 size, s32 tag);

void func_800496D0(s32 width, s32 height)
{
    void **cells;
    s32 count;
    s32 i;

    count = width * height;
    D_8009F790 = height;
    D_8009F78C = width;
    cells = func_80017EBC(count * 4, 2);
    D_8009F780 = cells;
    for (i = count - 1; i >= 0; i--) {
        cells[i] = 0;
    }
}
