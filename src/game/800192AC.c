#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Basis {
    struct Vec3s32 rows[3];
};

extern void func_80010974(struct Vec3s32 *a, struct Vec3s32 *b, struct Vec3s32 *out);
extern void func_8001A180(struct Vec3s32 *vector);

void func_800192AC(struct Vec3s32 *first, struct Vec3s32 *second, struct Basis *basis)
{
    func_80010974(first, second, &basis->rows[0]);
    func_8001A180(&basis->rows[0]);
    func_80010974(&basis->rows[0], first, &basis->rows[2]);
    basis->rows[1] = *first;
}
