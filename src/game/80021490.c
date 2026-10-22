#include "game/types.h"

struct Unk21490 {
    s32 channel;
    s32 arg1;
    s32 arg0;
    u8 unk0C[8];
    s32 mode;
};

extern s32 D_8009FA34;

extern void func_80098EE8(s32 channel);
extern void func_80098E88(s32 arg0, s32 arg1);
extern void func_80098E28(s32 arg0, s32 arg1);

void func_80021490(struct Unk21490 *request)
{
    D_8009FA34 = 1;
    func_80098EE8(request->channel);
    if (request->mode == 1) {
        func_80098E88(request->arg0, request->arg1);
    } else {
        func_80098E28(request->arg0, request->arg1);
    }
}
