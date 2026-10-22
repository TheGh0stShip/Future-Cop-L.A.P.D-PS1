#include "game/types.h"

extern u32 func_80097808(s32 register_index, u32 address);

u32 func_80098BB8(u32 address)
{
    if (address > 0x7FFF8) {
        return 0;
    }
    return func_80097808(0xD2, address);
}
