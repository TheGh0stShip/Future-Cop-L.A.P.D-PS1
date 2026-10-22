#include "game/types.h"

struct Unk50D14 {
    u8 unk00[0x26];
    s16 unk26;
    u8 unk28[0x14];
    u32 flags;
};

struct Unk50D14Arg {
    u8 unk00[0x0C];
    u8 unk0C;
};

extern void func_8004F188(struct Unk50D14 *object, struct Unk50D14Arg *arg);
extern s32 func_80050B54(struct Unk50D14 *object, s32 arg1, struct Unk50D14Arg *arg);
extern u8 func_8005027C(struct Unk50D14 *object, s32 arg1, struct Unk50D14Arg *arg);

u8 func_80050D14(struct Unk50D14 *object, s32 arg1, struct Unk50D14Arg *arg)
{
    if (arg->unk0C != 0) {
        func_8004F188(object, arg);
    }
    if (object->flags & 0x50) {
        if (object->unk26 > 0) {
            return func_80050B54(object, arg1, arg);
        }
        return 0;
    }
    return func_8005027C(object, arg1, arg);
}
