#include "game/types.h"

extern u16 *D_8009F5B0;

extern void *func_80017EBC(s32 size, s32 tag);
extern u32 func_80018818(u32 divisor);
extern u32 func_800187C8(u32 value);
extern s32 func_80095C14(u32 value);
extern void func_800197A8(s32 seed);

void func_80019828(void)
{
    s32 i;
    u32 input;
    u32 value;
    s32 shift;

    D_8009F5B0 = func_80017EBC(0x800, 2);
    for (input = 0, i = input; i < 0x400; input++) {
        value = func_800187C8(func_80018818(input));
        shift = 0x15;
        if (value != 0) {
            shift -= func_80095C14(value);
        }
        if (shift > 0) {
            value >>= shift;
        }
        value <<= 5;
        value |= shift;
        D_8009F5B0[i++] = value;
    }
    func_800197A8(0x457);
}
