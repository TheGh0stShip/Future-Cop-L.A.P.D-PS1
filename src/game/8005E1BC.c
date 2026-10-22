#include "game/types.h"

struct Pool {
    void *storage;
    void *active;
    void *free;
};

extern struct Pool D_800A22A0;
extern struct Pool D_800A22B0;
extern void *D_800A22F0[];
extern s16 D_800A22E8[];
extern s32 D_8009F374;

extern void func_8003BE2C(struct Pool *pool, s32 size, s32 count);

void func_8005E1BC(void)
{
    s32 i;

    D_800A22E8[0] = -1;
    func_8003BE2C(&D_800A22A0, 0x38, 0xA);
    func_8003BE2C(&D_800A22B0, 0x60, 0xA);
    for (i = 3; i >= 0; i--) {
        D_800A22F0[i] = 0;
    }
    D_8009F374 = 0;
}
