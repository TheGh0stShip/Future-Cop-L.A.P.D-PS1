#include "../../include/game/types.h"

struct GlobalState {
    u8 pad[0x8F4];
    s32 primary;
    s32 secondary;
};

extern struct GlobalState *D_8009AB08;

int func_8001F180(void)
{
    return D_8009AB08->primary != 0 || D_8009AB08->secondary != 0;
}
