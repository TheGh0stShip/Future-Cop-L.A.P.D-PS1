#include "game/types.h"

typedef void (*DecDctCallback)(void);

extern DecDctCallback func_800914E0(s32 channel, DecDctCallback callback);

DecDctCallback func_8009612C(DecDctCallback callback)
{
    return func_800914E0(1, callback);
}
