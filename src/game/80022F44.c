#include "game/types.h"

struct StreamState {
    u8 unk00[0x44];
    s32 base;
    u8 unk48[0x34];
    s32 offset;
};

extern struct StreamState D_800A0968;

extern void func_80098BB8(s32 address);
extern void func_80098A78(s32 mode);

void func_80022F44(void)
{
    func_80098BB8(D_800A0968.base + D_800A0968.offset);
    func_80098A78(1);
}
