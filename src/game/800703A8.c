#include "game/types.h"

struct Unk703A8 {
    s32 words[0x16];
};

extern u8 func_8003C2A4(struct Unk703A8 *data);
extern void *func_80017EBC(s32 size, s32 tag);
extern void func_8003C404(struct Unk703A8 *data);

void func_800703A8(struct Unk703A8 *source)
{
    struct Unk703A8 *copy;

    if (func_8003C2A4(source) != 0) {
        return;
    }
    copy = func_80017EBC(0x58, 2);
    if (copy == 0) {
        return;
    }
    *copy = *source;
    func_8003C404(copy);
}
