#include "game/types.h"

extern u8 D_8009AFA0[];

extern void func_8001E318(u8 *destination, u8 *source, s32 size);

void func_8005A09C(u8 *destination)
{
    func_8001E318(destination, D_8009AFA0, 0x14);
}
