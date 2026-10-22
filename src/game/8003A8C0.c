#include "game/types.h"

struct Unk3A8C0Args {
    s32 arg0;
    s32 arg2;
    s32 arg3;
    u8 arg1;
};

struct Unk3A8C0Request {
    void (*handler)(void);
    s32 unk04;
    struct Unk3A8C0Args *args;
    s32 unk0C;
};

extern void D_8003B2D0(void);
extern void func_8004D4D4(struct Unk3A8C0Request *request);

void func_8003A8C0(s32 arg0, u8 arg1, s32 arg2, s32 arg3)
{
    struct Unk3A8C0Request request;
    struct Unk3A8C0Args args;

    request.handler = D_8003B2D0;
    request.unk04 = 0;
    request.unk0C = 0;
    request.args = &args;
    args.arg0 = arg0;
    args.arg1 = arg1;
    args.arg2 = arg2;
    args.arg3 = arg3;
    func_8004D4D4(&request);
}
