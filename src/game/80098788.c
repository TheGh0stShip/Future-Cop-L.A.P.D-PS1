#include "game/types.h"

extern u32 func_800987B8(s32 on_off, u32 voice_bits, s32 low_register,
                         s32 high_register);

u32 func_80098788(s32 on_off, u32 voice_bits)
{
    return func_800987B8(on_off, voice_bits, 0xCC, 0xCD);
}
