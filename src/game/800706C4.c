#include "game/types.h"

struct Unk706C4 {
    s32 unk00;
    u8 unk04[0x18];
    s32 unk1C;
    s32 unk20;
};

extern u8 *func_80019C38(s32 arg0, s32 arg1, u32 tag, s32 arg3);
extern void func_800703A8(u8 *data);
extern void func_800181D4(void *block);

void func_800706C4(struct Unk706C4 *resource)
{
    func_800703A8(func_80019C38(resource->unk00, resource->unk20, 0x74414354,
                                resource->unk1C) - 4);
    func_800181D4(resource);
}
