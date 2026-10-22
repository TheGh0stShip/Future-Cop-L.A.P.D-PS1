#include "game/types.h"

extern void func_80021508(void);
extern void func_80022CA8(void);
extern s32 func_80021554(s32 *arguments);
extern void func_80021C80(void);

s32 func_80021C20(s32 first, ...)
{
    s32 result;

    func_80021508();
    func_80022CA8();
    result = func_80021554(&first);
    if (result != 0) {
        func_80021C80();
    }
    return result;
}
