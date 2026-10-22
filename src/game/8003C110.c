#include "game/types.h"

struct Unk3C110 {
    s32 unk00;
    u8 unk04[0x1C];
    s32 unk20;
};

extern void func_8003BFD0(s32 arg0, s32 arg1);
extern void func_800181D4(void *block);

void func_8003C110(struct Unk3C110 *resource)
{
    func_8003BFD0(resource->unk00, resource->unk20);
    func_800181D4(resource);
}
