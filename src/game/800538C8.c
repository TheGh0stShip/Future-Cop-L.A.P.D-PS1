#include "game/types.h"

extern s32 func_8001E7B8(s32 *x, s32 *y, s32 controller, s32 arg3);

s32 func_800538C8(u8 *context, u32 *flags, s32 *verticalMagnitude,
                  s32 *horizontalMagnitude)
{
    s32 x;
    s32 y;
    s32 status;
    u32 direction;
    s32 sign;

    if ((*flags & 0xF0000000) == 0) {
        if (*(s8 *)(context + 0xC0) != 0) {
            goto return0;
        }
        if (*(s8 *)(context + 0x310) == 0) {
            status = func_8001E7B8(&x, &y, *(s8 *)(context + 0xC1), 0);
            if ((u8)status == 0) {
                goto return0;
            }

            direction = 0;
            x -= 0x80;
            y -= 0x80;
            if ((u32)(x + 0x27) < 0x4F) {
                x = 0;
            } else if (x >= 0xE1) {
                x = 0x100;
            } else if (x < -0xE0) {
                x = -0x100;
            }

            if (x > 0) {
                direction |= 0x20000000;
            } else if (x < 0) {
                direction |= 0x10000000;
            }

            if ((u32)(y + 0x27) < 0x4F) {
                y = 0;
            } else if (y >= 0xE1) {
                y = 0x100;
            } else if (y < -0xE0) {
                y = -0x100;
            }

            if (y > 0) {
                direction |= 0x80000000;
            } else if (y < 0) {
                direction |= 0x40000000;
            }

            *flags |= direction;
            sign = y >> 31;
            *verticalMagnitude = (y + sign) ^ sign;
            sign = x >> 31;
            *horizontalMagnitude = (x + sign) ^ sign;
            return (u8)status;
        }
    }

return0:
    return 0;
}
