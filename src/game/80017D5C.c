#include "game/types.h"

extern s32 *D_8009F590;
extern s32 D_8009F594;
extern s32 D_8009F598;

s32 func_80017D5C(s32 size)
{
    s32 i;
    s32 *block;

    block = &D_8009F590[D_8009F598];
    for (i = D_8009F598; i < D_8009F594; i += 2, block += 2) {
        if (block[1] - block[0] >= size) {
            return i;
        }
    }
    block = &D_8009F590[1];
    for (i = 1; i < D_8009F598; i += 2, block += 2) {
        if (block[1] - block[0] >= size) {
            return i;
        }
    }
    return -1;
}
