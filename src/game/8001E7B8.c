#include "game/types.h"

struct PadBuffer {
    u8 unk00[4];
    u8 rightX;
    u8 rightY;
    u8 leftX;
    u8 leftY;
    u8 unk08[0x1C];
};

extern struct PadBuffer D_800A78F0[];

extern s32 func_8001EF34(s32 port);

s32 func_8001E7B8(s32 *x, s32 *y, s32 port, s32 stick)
{
    if (func_8001EF34(port) != 7) {
        return 0;
    }
    if (stick == 1) {
        *x = D_800A78F0[port].rightX;
        *y = D_800A78F0[port].rightY;
    } else {
        *x = D_800A78F0[port].leftX;
        *y = D_800A78F0[port].leftY;
    }
    return 1;
}
