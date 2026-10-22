#include "game/types.h"

extern void func_8001E9AC(s32 port, s32 enable);
extern void func_8001EA5C(s32 port, s32 level);

void func_8001EBA8(s32 port)
{
    s32 clamped;

    if (port >= 0) {
        if (port < 2) {
            clamped = port;
        } else {
            clamped = 1;
        }
    } else {
        clamped = 0;
    }
    port = clamped;
    func_8001E9AC(port, 0);
    func_8001EA5C(port, 0);
}
