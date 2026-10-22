#include "game/types.h"

extern void func_800402DC(void *object, u8 kind, s32 arg2, s32 arg3, s32 arg4);
extern void func_8003FB90(void *object, u8 kind);

void func_8003FD10(s32 message, void *object, s32 kind)
{
    switch (message) {
    case 0x64:
        func_800402DC(object, kind, 0, 0, 0);
        break;
    case 0x65:
        func_8003FB90(object, kind);
        break;
    }
}
