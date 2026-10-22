#include "game/types.h"

struct Unk44770 {
    s32 words[0x18];
};

extern u8 func_8003C2A4(struct Unk44770 *data);
extern void *func_80017EBC(s32 size, s32 tag);
extern void func_8003C404(struct Unk44770 *data);

void func_80044770(struct Unk44770 *source)
{
    struct Unk44770 *copy;

    if (func_8003C2A4(source) != 0) {
        return;
    }
    copy = func_80017EBC(0x60, 2);
    *copy = *source;
    func_8003C404(copy);
}
