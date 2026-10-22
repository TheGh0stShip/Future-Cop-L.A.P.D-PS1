#include "../../include/game/types.h"

struct DeviceState {
    u8 pad_00[0x46];
    u8 device;
    u8 pad_47[0x9F];
    u16 ready;
};

int func_8008C858(struct DeviceState *state)
{
    if (state->ready == 0) {
        return 0;
    }
    return state->device != 0xFF;
}
