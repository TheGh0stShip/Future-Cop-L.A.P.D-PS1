#include "game/types.h"

struct FontEntry {
    u8 data[0x11C];
};

extern struct FontEntry D_800A19A0[];
extern s32 D_8009F364;
extern s32 D_8009F76C;
extern struct FontEntry *D_8009F35C;

extern void func_80048DEC(void);

s32 func_80048E28(s32 index)
{
    s32 result;

    if (index < D_8009F364) {
        goto valid;
    }
    result = 0;
    goto done;
valid:
    D_8009F76C = index;
    D_8009F35C = &D_800A19A0[index];
    func_80048DEC();
    result = 1;
done:
    return result;
}
