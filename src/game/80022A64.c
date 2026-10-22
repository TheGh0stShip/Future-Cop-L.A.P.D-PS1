#include "game/types.h"

struct StreamChannel {
    s32 id;
    s32 voice;
    u8 unk08[0x1C];
};

struct StreamState {
    u8 flag;
    u8 unk01[3];
    s32 active;
    s32 unk08;
    u8 unk0C[4];
    s16 unk10;
    u8 unk12[2];
    u32 channelCount;
    u8 unk18[0x0C];
    struct StreamChannel channels[3];
};

extern struct StreamState D_800A0968;

extern void func_80022F7C(void);
extern void func_80098BF8(s32 mode);
extern void func_8002129C(void);
extern void func_80022240(s32 owner);
extern void func_80046A34(s32 handle);

s32 func_80022A64(void)
{
    u32 i;

    if (D_800A0968.active == 0) {
        return 0;
    }
    D_800A0968.unk10 = 0;
    func_80022F7C();
    func_80098BF8(0);
    D_800A0968.flag = 0;
    D_800A0968.active = 0;
    D_800A0968.unk08 = 0;
    func_8002129C();
    for (i = 0; i < D_800A0968.channelCount; i++) {
        if (D_800A0968.channels[i].id != -1) {
            func_80022240(D_800A0968.channels[i].id);
            func_80046A34(D_800A0968.channels[i].voice);
        }
    }
    return 0;
}
