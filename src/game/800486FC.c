#include "game/types.h"

extern s32 D_8009FA00[];
extern s32 D_8009FA0C[];
extern u8 D_8009F9B4[];
extern s32 D_8009FAB8;
extern s32 D_8009FAC8;

extern void func_80049260(void);
extern s32 func_80029748(u8 *name);
extern void func_800487CC(u8 force);
extern void func_800485AC(s32 value);

void func_800486FC(u8 enable)
{
    if (D_8009FA00[0] == 4) {
        func_80049260();
    }
    D_8009FAC8 = func_80029748(D_8009F9B4);
    func_800487CC(0);
    if (D_8009FAB8 != 0 && D_8009FAB8 == 2 && enable && D_8009FA0C[0] == 1) {
        func_800485AC(0);
    }
}
