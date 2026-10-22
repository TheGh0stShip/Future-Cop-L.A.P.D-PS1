#include "game/types.h"

extern u8 D_8009B724[];
extern u8 D_8009F3C0;
extern u8 D_8009F880;
extern u8 D_8009F881;
extern u8 D_8009F882;
extern u8 D_8009F889;
extern u8 D_8009F458;
extern u8 D_8009F3F1;
extern u8 *D_8009F884;
extern u8 D_8009F888;

void func_800776B8(void)
{
    D_8009F880 = 0;
    D_8009F881 = 0;
    D_8009F889 = 0;
    if (D_8009F3C0 != 0) {
        D_8009F882 = 7;
        D_8009F458 = 8;
        D_8009F3F1 = 8;
    } else {
        D_8009F882 = 0;
    }
    D_8009F884 = D_8009B724;
    D_8009F888 = 8;
}
