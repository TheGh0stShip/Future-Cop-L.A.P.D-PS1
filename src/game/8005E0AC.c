#include "game/types.h"

struct Pool {
    void *storage;
    void *active;
    void *free;
};

extern struct Pool D_800A22A0;
extern struct Pool D_800A22B0;
extern void *D_800A22F0[];

extern void func_8005B3D4(void *object);
extern void func_8005BD20(struct Pool *pool, void *object);
extern void func_8005C0C0(struct Pool *pool, void *object);
extern u8 func_8005D8F4(void *effect, void *object);
extern void func_8005E074(void *effect);

void func_8005E0AC(void *object)
{
    s32 i;

    func_8005B3D4(object);
    func_8005BD20(&D_800A22A0, object);
    func_8005C0C0(&D_800A22B0, object);
    for (i = 0; i < 4; i++) {
        if (D_800A22F0[i] != 0 && func_8005D8F4(D_800A22F0[i], object) != 1) {
            func_8005E074(D_800A22F0[i]);
            D_800A22F0[i] = 0;
        }
    }
}
