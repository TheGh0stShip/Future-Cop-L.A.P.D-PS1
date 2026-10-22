#include "game/types.h"

typedef void (*SpuIrqCallback)(void);

extern void func_800914B0(s32 interrupt, SpuIrqCallback callback);

void func_80098C38(SpuIrqCallback callback)
{
    func_800914B0(9, callback);
}
