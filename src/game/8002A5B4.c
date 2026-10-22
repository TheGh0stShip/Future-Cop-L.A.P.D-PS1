#include "../../include/game/types.h"

struct ShortQueue {
    u8 pad_00[0xA];
    s8 values[8];
    u8 active;
};

void func_8002A5B4(struct ShortQueue *queue, int index)
{
    int next;

    for (next = index + 1; next < 4; next++) {
        queue->values[next * 2 - 2] = queue->values[next * 2];
        queue->values[next * 2 - 1] = queue->values[next * 2 + 1];
    }
    queue->active = 0;
}
