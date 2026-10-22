#include "game/types.h"

struct Unk4CAE4State {
    u8 unk00[0x0C];
    s32 mode0C;
    u8 unk10[0x4C];
    s32 mode5C;
};

extern s32 D_8009F7A0;
extern s32 D_8009F7A4;
extern s32 D_8009F7A8;
extern u8 D_8009F7AC;
extern s32 D_8009F9F4[];
extern s32 D_8009FA40;
extern s32 D_8009FA88;
extern struct Unk4CAE4State *D_8009FA94[];
extern s32 D_800A6F60[];
extern s32 D_800A8F90[];

void func_8004CAE4(void)
{
    register s32 colorF8F800 __asm__("$13");
    register s32 colorF800F8 __asm__("$14");
    register s32 colorF8F8F8 __asm__("$12");
    register s32 color787800 __asm__("$11");
    register s32 color780078 __asm__("$15");
    register s32 color787878 __asm__("$10");
    register s32 color8080F8 __asm__("$18");
    register s32 colorF88070 __asm__("$17");
    register s32 color80F880 __asm__("$16");
    register s32 *entityTable __asm__("$9");
    s32 limit;

    colorF8F800 = 0xF8F800;
    colorF800F8 = 0xF800F8;
    colorF8F8F8 = 0xF8F8F8;
    color787800 = 0x787800;
    color780078 = 0x780078;
    color787878 = 0x787878;
    color8080F8 = 0x8080F8;
    colorF88070 = 0xF88070;
    color80F880 = 0x80F880;
    entityTable = D_800A8F90;

    entityTable[0] = 0;
    D_800A6F60[0] = 0;
    D_8009F7A4 = 0;
    D_8009FA40 = 0;
    D_8009FA88 = 0;
    D_8009F7A0 = 0;

    D_800A6F60[1] = 0xF8;
    D_800A6F60[2] = 0xF80000;
    D_800A6F60[3] = 0xF800;
    D_800A6F60[4] = colorF8F800;
    D_800A6F60[5] = 0xF8F8;
    D_800A6F60[6] = colorF800F8;
    D_800A6F60[7] = colorF8F8F8;
    D_800A6F60[8] = 0x78F8;
    D_800A6F60[9] = 0x78;
    D_800A6F60[10] = 0x780000;
    D_800A6F60[11] = 0x7800;
    D_800A6F60[12] = color787800;
    D_800A6F60[13] = 0x7878;
    D_800A6F60[14] = color780078;
    D_800A6F60[15] = color787878;
    D_800A6F60[16] = 0x3878;
    D_800A6F60[17] = 0xF8;
    D_800A6F60[18] = 0xF80000;
    D_800A6F60[19] = 0xF800;
    D_800A6F60[20] = color8080F8;
    D_800A6F60[21] = colorF88070;
    D_800A6F60[22] = color80F880;
    D_800A6F60[23] = 0xF8;
    D_800A6F60[24] = 0xF80000;
    D_800A6F60[25] = 0xF800;
    D_800A6F60[26] = colorF8F800;
    D_800A6F60[27] = 0xF8F8;
    D_800A6F60[28] = colorF800F8;
    D_800A6F60[29] = colorF8F8F8;
    D_800A6F60[30] = 0x78F8;
    D_800A6F60[31] = color787878;
    D_8009F9F4[0] = 0;

    D_8009F7AC = 1;
    D_8009F7A8 = 0;
    if (D_8009FA94[0]->mode5C != 1) {
        goto check_secondary;
    }
    limit = 0xFE;
    goto store_limit;
check_secondary:
    if (D_8009FA94[0]->mode0C == 2) {
        limit = 0x12C;
    } else {
        limit = 0xC8;
    }
store_limit:
    entityTable[1] = limit;
}
