#include "game/types.h"

struct CardContext {
    u8 unk000[0x8C4];
    s32 lockCount;
    u8 unk8C8[4];
    void (*savedCallback)(void);
};

extern struct CardContext *D_8009AB08;

extern void func_8008C9C0(void);
extern s32 func_80091510(void (*callback)(void));
extern void func_8008C9E0(void);

void func_8002059C(void)
{
    if (--D_8009AB08->lockCount == 0) {
        func_8008C9C0();
        func_80091510(D_8009AB08->savedCallback);
        D_8009AB08->savedCallback = 0;
        func_8008C9E0();
    }
}
