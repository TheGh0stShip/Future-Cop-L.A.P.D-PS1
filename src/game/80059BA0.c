#include "game/types.h"

extern s32 D_8009F238;

extern s32 func_8001E110(s32 name, s32 arg1, void **data, s32 *size, s32 arg4);
extern void func_800181D4(void *block);

void *func_80059BA0(s32 name)
{
    void *data;
    s32 size;

    if (func_8001E110(name, D_8009F238, &data, &size, 0) > 0) {
        return data;
    }
    if (data != 0) {
        func_800181D4(data);
    }
    return 0;
}
