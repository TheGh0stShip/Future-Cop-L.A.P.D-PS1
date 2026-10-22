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
    if (index >= D_8009F364) {
        return 0;
    }
    D_8009F76C = index;
    D_8009F35C = &D_800A19A0[index];
    func_80048DEC();
    return 1;
}
