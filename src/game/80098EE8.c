#include "game/types.h"

extern u16 D_8009EEBC;
extern u32 D_8009EECC;

extern u32 func_80097808(s32 register_index, u32 address);

u32 func_80098EE8(u32 address)
{
    u32 offset;

    offset = address - 0x1010;
    if (offset > 0x7EFE8) {
        return 0;
    }

    D_8009EEBC = func_80097808(-1, address);
    return (u32)D_8009EEBC << D_8009EECC;
}
