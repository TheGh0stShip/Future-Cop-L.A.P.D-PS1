#include "game/types.h"

extern s32 D_8009F464;
extern void *D_8009F460;
extern s32 D_8009F45C;
extern void **D_8009F3F4;

extern void func_800898E8(void);
extern void func_8007FACC(void);
extern void func_8007FA48(void);
extern void func_800181D4(void *block);

void func_8007852C(void)
{
    u32 i;

    if (D_8009F464 == 0) {
        return;
    }
    func_800898E8();
    func_8007FACC();
    func_8007FA48();
    if (D_8009F460 != 0) {
        func_800181D4(D_8009F460);
        D_8009F460 = 0;
        D_8009F45C = 0;
    }
    D_8009F464 = 0;
    if (D_8009F3F4 != 0) {
        for (i = 0; i < 0x1E; i++) {
            if (D_8009F3F4[i] != 0) {
                func_800181D4(D_8009F3F4[i]);
                D_8009F3F4[i] = 0;
            }
        }
        func_800181D4(D_8009F3F4);
        D_8009F3F4 = 0;
    }
}
