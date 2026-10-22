#include "game/types.h"

struct Unk6AAEC {
    u8 id;
    u8 unk01[0x53];
};

extern s32 D_8009F848;
extern struct Unk6AAEC D_800A3340[];

struct Unk6AAEC *func_8006AAEC(u8 id)
{
    s32 i;

    for (i = 0; i < D_8009F848; i++) {
        if (D_800A3340[i].id == id) {
            return &D_800A3340[i];
        }
    }
    return 0;
}
