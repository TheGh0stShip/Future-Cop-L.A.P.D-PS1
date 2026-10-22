#include "game/types.h"

struct Unk78210Mode {
    u8 unk00[0x0C];
    s32 players;
    u8 unk10[0x4C];
    s32 split;
};

struct Unk78210Panel {
    u8 unk00[5];
    u8 visible;
    u8 unk06[0x2A];
};

extern struct Unk78210Mode *D_8009FA94[];
extern struct Unk78210Panel D_800A3868;
extern struct Unk78210Panel D_800A3898;
extern struct Unk78210Panel D_800A38C8;
extern u8 D_800A377D[];
extern s32 D_8009F8E0;
extern s32 D_8009F8E4;

extern void func_8007A168(void);
extern void func_80078650(struct Unk78210Panel *panel, s32 arg1);

void func_80078210(void)
{
    func_8007A168();
    if (D_8009FA94[0]->players == 1) {
        if (D_8009FA94[0]->split == 1) {
            func_80078650(&D_800A38C8, 0);
        } else {
            func_80078650(&D_800A3868, 0);
            D_800A3868.visible = 1;
        }
        D_800A377D[0] = 1;
    } else {
        *(volatile s32 *)&D_8009F8E0 = 0;
        *(volatile s32 *)&D_8009F8E4 = 0;
        *(volatile u8 *)&D_800A377D[0] = 0;
        if (((volatile struct Unk78210Mode *)D_8009FA94[0])->split == 1) {
            func_80078650(&D_800A3898, 0);
        } else {
            func_80078650(&D_800A3868, 0);
            D_800A3868.visible = 0;
        }
    }
}
