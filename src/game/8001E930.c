#include "../../include/game/types.h"

struct InnerState {
    u8 pad[0xC1];
    s8 index;
};

struct OuterState {
    u8 pad[0xAC];
    struct InnerState *inner;
};

extern void func_8001E9AC(int index);

void func_8001E930(struct OuterState *state)
{
    int index = state != 0 ? state->inner->index : 0;
    func_8001E9AC(index);
}
