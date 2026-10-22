#include "../../include/game/types.h"

extern u8 D_800A3C20[];

int func_8008C3B4(void *address)
{
    int index;
    u8 *current = D_800A3C20;
    int result = 0x10;

    for (index = 0; index < 2; index++) {
        if (address == current) {
            return result;
        }
        result += 0x10;
        current += 0xF0;
    }
    return 0xFF;
}
