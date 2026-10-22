#include "game/types.h"

typedef void (*SpuIrqCallback)(void);

extern SpuIrqCallback D_8009EEE0;
extern void func_80098C38(SpuIrqCallback callback);

SpuIrqCallback func_80098BF8(SpuIrqCallback callback)
{
    SpuIrqCallback previous;

    previous = D_8009EEE0;
    if (callback != previous) {
        D_8009EEE0 = callback;
        func_80098C38(callback);
    }
    return previous;
}
