#include "game/types.h"

typedef void (*CdDataCallback)(void);

extern CdDataCallback func_800914E0(s32 channel, CdDataCallback callback);

CdDataCallback func_8008E5D8(CdDataCallback callback)
{
    return func_800914E0(3, callback);
}
