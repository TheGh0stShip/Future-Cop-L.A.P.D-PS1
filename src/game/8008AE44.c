#include "../../include/game/types.h"

struct StatusState {
    u8 pad[4];
    volatile u16 status;
};

extern struct StatusState *D_8009C78C;

void func_8008AE44(void)
{
    while ((D_8009C78C->status & 2) == 0) {
    }
}
