#include "game/types.h"

extern void func_8001846C(u32 angle, s32 *sine, s32 *cosine);

#define FIXED_MUL(a, b) (((a) >> 2) * ((b) >> 2) >> 12)

s32 *func_800188D0(u32 yAngle, u32 xAngle, u32 zAngle, s32 *matrix)
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
            matrix[4] = 0x10000;
            matrix[7] = 0;
            matrix[5] = 0;
            matrix[3] = 0;
            matrix[1] = 0;
            if (yAngle != 0) {
                func_8001846C(yAngle, &sy, &cy);
                sxcy = cy;
                matrix[0] = sxcy;
                matrix[8] = sxcy;
                matrix[6] = sy;
                matrix[2] = -sy;
            } else {
                matrix[8] = 0x10000;
                matrix[0] = 0x10000;
                matrix[6] = 0;
                matrix[2] = 0;
            }
        } else {
            func_8001846C(xAngle, &sx, &cx);
            if (yAngle == 0) {
                matrix[0] = 0x10000;
                matrix[6] = 0;
                matrix[3] = 0;
                matrix[2] = 0;
                matrix[1] = 0;
                matrix[5] = sx;
                matrix[4] = cx;
                matrix[8] = cx;
                matrix[7] = -sx;
            } else {
                func_8001846C(yAngle, &sy, &cy);
                matrix[0] = cy;
                matrix[1] = 0;
                matrix[2] = -sy;
                matrix[4] = cx;
                matrix[7] = -sx;
                sx >>= 2;
                cx >>= 2;
                sy >>= 2;
                cy >>= 2;
                matrix[3] = (sx * sy) >> 12;
                matrix[5] = (sx * cy) >> 12;
                matrix[6] = (cx * sy) >> 12;
                matrix[8] = (cx * cy) >> 12;
            }
        }
    } else {
        func_8001846C(zAngle, &sz, &cz);
        if (xAngle == 0) {
            if (yAngle == 0) {
                matrix[6] = 0;
                matrix[0] = cz;
                matrix[8] = 0x10000;
                matrix[7] = 0;
                matrix[5] = 0;
                matrix[2] = 0;
                matrix[4] = cz;
                matrix[1] = sz;
                matrix[3] = -sz;
            } else {
                func_8001846C(yAngle, &sy, &cy);
                matrix[6] = sy;
                matrix[1] = sz;
                matrix[4] = cz;
                matrix[7] = 0;
                matrix[8] = cy;
                sz >>= 2;
                cz >>= 2;
                sy >>= 2;
                cy >>= 2;
                matrix[0] = (cz * cy) >> 12;
                matrix[2] = -(cz * sy) >> 12;
                matrix[3] = -(sz * cy) >> 12;
                matrix[5] = (sz * sy) >> 12;
            }
        } else {
            func_8001846C(xAngle, &sx, &cx);
            if (yAngle == 0) {
                matrix[0] = cz;
                matrix[3] = -sz;
                matrix[6] = 0;
                matrix[7] = -sx;
                matrix[8] = cx;
                sz >>= 2;
                cz >>= 2;
                sx >>= 2;
                cx >>= 2;
                matrix[1] = (sz * cx) >> 12;
                matrix[2] = (sz * sx) >> 12;
                matrix[4] = (cz * cx) >> 12;
                matrix[5] = (cz * sx) >> 12;
            } else {
                func_8001846C(yAngle, &sy, &cy);
                matrix[7] = -sx;
                sz >>= 2;
                sx >>= 2;
                sxsy = (sz * sx) >> 14;
                cz >>= 2;
                cy >>= 2;
                sy >>= 2;
                cx >>= 2;
                sxcy = (sx * cz) >> 14;
                matrix[0] = (cz * cy + sxsy * sy) >> 12;
                matrix[1] = (sz * cx) >> 12;
                matrix[2] = (sxsy * cy - cz * sy) >> 12;
                matrix[4] = (cz * cx) >> 12;
                matrix[6] = (cx * sy) >> 12;
                matrix[8] = (cx * cy) >> 12;
                matrix[3] = (sxcy * sy - sz * cy) >> 12;
                matrix[5] = (sz * sy + sxcy * cy) >> 12;
            }
        }
    }
    return matrix;
}
