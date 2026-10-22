#include "game/types.h"

typedef void (*SpuDmaCallback)(void);

extern void func_800914E0(s32 channel, SpuDmaCallback callback);

void func_800979F8(SpuDmaCallback callback)
{
    func_800914E0(4, callback);
}
