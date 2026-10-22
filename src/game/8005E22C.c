#include "game/types.h"

struct Pool {
    void *storage;
    void *active;
    void *free;
};

extern struct Pool D_800A22A0;
extern struct Pool D_800A22B0;
extern void *D_800A22F0[];

extern void func_8003BDEC(struct Pool *pool);
extern void func_8005E074(void *effect);

void func_8005E22C(void)
{
    s32 i;

    func_8003BDEC(&D_800A22A0);
    func_8003BDEC(&D_800A22B0);
    for (i = 0; i < 4; i++) {
        if (D_800A22F0[i] != 0) {
            func_8005E074(D_800A22F0[i]);
        }
    }
}
