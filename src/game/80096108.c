#include "game/types.h"

typedef void (*DecDctCallback)(void);

extern DecDctCallback func_800914E0(s32 channel, DecDctCallback callback);

DecDctCallback func_80096108(DecDctCallback callback)
{
    return func_800914E0(0, callback);
}
