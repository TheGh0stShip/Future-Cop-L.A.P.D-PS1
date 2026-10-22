#include "game/types.h"

struct KeyValue {
    s32 key;
    s32 value;
};

s32 func_80021740(struct KeyValue *table, s32 key, s32 *value, s32 *index)
{
    s32 found;
    s32 i;

    found = 0;
    if (table != 0 && table->key != 0) {
    i = 0;
    do {
        if (table->key == key) {
            found = 1;
            if (value != 0) {
                *value = table->value;
            }
            if (index != 0) {
                *index = i;
            }
            break;
        }
        table++;
        i++;
    } while (table->key != 0);
    }
    return found;
}
