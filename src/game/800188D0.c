#include "game/types.h"

struct Matrix3x3L {
    s32 m[3][3];
};

extern void func_8001846C(u32 angle, s32 *sine, s32 *cosine);

#define FIXED_MUL(a, b) (((a) >> 2) * ((b) >> 2) >> 12)

struct Matrix3x3L *func_800188D0(u32 yAngle, u32 xAngle, u32 zAngle,
                                 struct Matrix3x3L *matrix)
{
    s32 sy;
    s32 cy;
    s32 sx;
    s32 cx;
    s32 sz;
    s32 cz;
    s32 sxsy;
    s32 sxcy;

    zAngle &= 0xFFF;
    xAngle &= 0xFFF;
    yAngle &= 0xFFF;

    if (zAngle == 0) {
        if (xAngle == 0) {
            matrix->m[1][1] = 0x10000;
            matrix->m[2][1] = 0;
            matrix->m[1][2] = 0;
            matrix->m[1][0] = 0;
            matrix->m[0][1] = 0;
            if (yAngle != 0) {
                func_8001846C(yAngle, &sy, &cy);
                matrix->m[0][0] = cy;
                matrix->m[2][2] = cy;
                matrix->m[2][0] = sy;
                matrix->m[0][2] = -sy;
            } else {
                matrix->m[2][2] = 0x10000;
                matrix->m[0][0] = 0x10000;
                matrix->m[2][0] = 0;
                matrix->m[0][2] = 0;
            }
        } else {
            func_8001846C(xAngle, &sx, &cx);
            if (yAngle == 0) {
                matrix->m[0][0] = 0x10000;
                matrix->m[2][0] = 0;
                matrix->m[1][0] = 0;
                matrix->m[0][2] = 0;
                matrix->m[0][1] = 0;
                matrix->m[1][2] = sx;
                matrix->m[1][1] = cx;
                matrix->m[2][2] = cx;
                matrix->m[2][1] = -sx;
            } else {
                func_8001846C(yAngle, &sy, &cy);
                matrix->m[0][0] = cy;
                matrix->m[0][1] = 0;
                matrix->m[0][2] = -sy;
                matrix->m[1][0] = FIXED_MUL(sx, sy);
                matrix->m[1][1] = cx;
                matrix->m[1][2] = FIXED_MUL(sx, cy);
                matrix->m[2][0] = FIXED_MUL(cx, sy);
                matrix->m[2][1] = -sx;
                matrix->m[2][2] = FIXED_MUL(cx, cy);
            }
        }
    } else {
        func_8001846C(zAngle, &sz, &cz);
        if (xAngle == 0) {
            if (yAngle == 0) {
                matrix->m[2][0] = 0;
                matrix->m[0][0] = cz;
                matrix->m[2][2] = 0x10000;
                matrix->m[2][1] = 0;
                matrix->m[1][2] = 0;
                matrix->m[0][2] = 0;
                matrix->m[1][1] = cz;
                matrix->m[0][1] = sz;
                matrix->m[1][0] = -sz;
            } else {
                func_8001846C(yAngle, &sy, &cy);
                matrix->m[2][0] = sy;
                matrix->m[0][0] = FIXED_MUL(cz, cy);
                matrix->m[0][1] = sz;
                matrix->m[0][2] = -FIXED_MUL(cz, sy);
                matrix->m[1][0] = -FIXED_MUL(sz, cy);
                matrix->m[1][1] = cz;
                matrix->m[1][2] = FIXED_MUL(sz, sy);
                matrix->m[2][1] = 0;
                matrix->m[2][2] = cy;
            }
        } else {
            func_8001846C(xAngle, &sx, &cx);
            if (yAngle == 0) {
                matrix->m[0][0] = cz;
                matrix->m[0][1] = FIXED_MUL(sz, cx);
                matrix->m[0][2] = FIXED_MUL(sz, sx);
                matrix->m[1][0] = -sz;
                matrix->m[1][1] = FIXED_MUL(cz, cx);
                matrix->m[1][2] = FIXED_MUL(cz, sx);
                matrix->m[2][0] = 0;
                matrix->m[2][1] = -sx;
                matrix->m[2][2] = cx;
            } else {
                func_8001846C(yAngle, &sy, &cy);
                matrix->m[2][1] = -sx;
                sz >>= 2;
                sx >>= 2;
                sxsy = (sz * sx) >> 14;
                cz >>= 2;
                cy >>= 2;
                sy >>= 2;
                cx >>= 2;
                sxcy = (sx * cz) >> 14;
                matrix->m[0][0] = (cz * cy + sxsy * sy) >> 12;
                matrix->m[0][1] = (sz * cx) >> 12;
                matrix->m[0][2] = (sxsy * cy - cz * sy) >> 12;
                matrix->m[1][0] = (sxcy * sy - sz * cy) >> 12;
                matrix->m[1][1] = (cz * cx) >> 12;
                matrix->m[1][2] = (sz * sy + sxcy * cy) >> 12;
                matrix->m[2][0] = (cx * sy) >> 12;
                matrix->m[2][2] = (cx * cy) >> 12;
            }
        }
    }
    return matrix;
}
