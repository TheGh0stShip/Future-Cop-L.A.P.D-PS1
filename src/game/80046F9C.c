#include "game/types.h"

struct SoundBank {
    s32 samples[4];
    u8 count;
    u8 cursor;
    u8 voices;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 key;
    u8 unk17;
    s32 head;
};

extern volatile u8 D_8009F340;
extern struct SoundBank D_800A7938[];

extern s32 func_800458E8(s32 arg0, s32 arg1, s8 *mix, s32 arg3);
extern s32 func_80046B04(s32 volume, s32 arg1, s32 arg2, u32 index);
extern void func_800460D4(s8 *mix);
extern u8 func_8004772C(struct SoundBank *bank, s32 handle, s32 pan, s32 volume, s32 extra);
extern void func_80046698(s32 handle);

s32 func_80046F9C(s32 arg0, s32 arg1, u32 index, s32 arg3, s32 arg4)
{
    struct SoundBank *bank;
    register s32 handle __asm__("$17");
    register u32 indexValue __asm__("$18");
    register s32 arg4Value __asm__("$19");
    register u32 enabled __asm__("$2");
    s8 mix[0x18];

    enabled = D_8009F340;
    arg4Value = arg4;
    __asm__("" : "=r"(arg4Value) : "0"(arg4Value));
    indexValue = index;
    __asm__("" : "=r"(indexValue) : "0"(indexValue));
    handle = 0;
    __asm__("" : "=r"(handle) : "0"(handle));
    if (enabled == 0) {
        return handle;
    }
    if (indexValue >= 0x6E) {
        return handle;
    }
    bank = &D_800A7938[indexValue];
    if (bank->count == 0) {
        return handle;
    }
    if (func_800458E8(arg0, arg1, mix, arg3) == 0) {
        return handle;
    }
    handle = func_80046B04(mix[0], bank->unk13, (s8)arg4Value, indexValue);
    if (handle != 0) {
        func_800460D4(mix);
        if (func_8004772C(bank, handle, mix[1], mix[0], -1) == 0) {
            func_80046698(handle);
            handle = 0;
        }
    }
    return handle;
}
