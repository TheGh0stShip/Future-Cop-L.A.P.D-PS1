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
    u8 unk16;
    u8 enabled;
};

extern struct PadActuator D_800A6FF8[];
extern s32 D_8009FA2C;

void func_8001E9AC(s32 port, s32 enable)
{
    struct PadActuator *actuator;

    actuator = &D_800A6FF8[port];
    if (!actuator->enabled || actuator->smallIndex == 0xFF) {
        return;
    }
    if (enable) {
        actuator->motor[actuator->smallIndex] = 1;
        actuator->smallTimer = D_8009FA2C + 300;
    } else {
        actuator->motor[actuator->smallIndex] = 0;
        actuator->smallTimer = 0;
    }
}
