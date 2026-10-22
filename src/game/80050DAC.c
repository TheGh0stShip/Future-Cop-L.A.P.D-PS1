#include "game/types.h"

struct Unk50DACCell {
    s16 unk00;
    s16 x;
    s16 unk04;
    s16 z;
    s32 unk08;
};

struct Unk50DACMap {
    s16 unk00;
    s16 count;
    s32 unk04;
    struct Unk50DACCell cells[1];
};

s32 func_80050DAC(struct Unk50DACMap *map, s32 *position)
{
    s32 x;
    s32 z;
    s32 i;

    x = position[0] >> 8;
    z = position[2] >> 8;
    for (i = 0; i < map->count; i++) {
        if (map->cells[i].x == x && map->cells[i].z == z) {
            return i;
        }
    }
    return -1;
}
