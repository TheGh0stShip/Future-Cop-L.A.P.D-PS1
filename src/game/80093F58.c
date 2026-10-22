#include "../../include/game/types.h"

u32 func_80093F58(s32 dfe, s32 dtd, u32 tpage)
{
    return 0xE1000000
        | (dtd != 0 ? 0x200 : 0)
        | (dfe != 0 ? 0x400 : 0)
        | (tpage & 0x9FF);
}
