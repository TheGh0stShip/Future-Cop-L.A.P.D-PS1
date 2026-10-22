#include "game/types.h"

struct Unk63C20 {
    s32 unk00;
    u8 unk04[0x18];
    s32 unk1C;
    s32 unk20;
};

extern void func_80019A74(struct Unk63C20 *resource);
extern void func_800639D4(s32 arg0, s32 arg1, s32 arg2);

void func_80063C20(struct Unk63C20 *resource)
{
    func_80019A74(resource);
    func_800639D4(resource->unk00, resource->unk20, resource->unk1C);
}
