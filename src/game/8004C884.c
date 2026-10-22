#include "game/types.h"

struct Unk4C884 {
    s32 words[0x13];
};

extern u8 func_8003C2A4(struct Unk4C884 *data);
extern void *func_80017EBC(s32 size, s32 tag);
extern void func_8003C404(struct Unk4C884 *data);

void func_8004C884(struct Unk4C884 *source)
{
    struct Unk4C884 *copy;

    if (func_8003C2A4(source) != 0) {
        return;
    }
    copy = func_80017EBC(0x4C, 2);
    if (copy == 0) {
        return;
    }
    *copy = *source;
    func_8003C404(copy);
}
