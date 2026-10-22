#include "game/types.h"

struct PadActuator {
    u8 port;
    u8 unk01;
    u8 motor[2];
    u8 smallIndex;
    u8 largeIndex;
    u8 unk06[2];
    s32 smallTimer;
    s32 largeTimer;
    u8 unk10[6];
    u8 mode;
    u8 enabled;
};

extern struct PadActuator D_800A6FF8[];

extern void func_8001EBA8(s32 port);

void func_8001EC04(s32 port, s32 enable)
{
    s32 clamped;

    if (port >= 0) {
        if (port < 2) {
            clamped = port;
        } else {
            clamped = 1;
        }
    } else {
        clamped = 0;
    }
    port = clamped;
    if (enable) {
        D_800A6FF8[port].enabled = 1;
    } else {
        func_8001EBA8(port);
        D_800A6FF8[port].enabled = 0;
    }
}
