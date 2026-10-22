#include "../../include/game/types.h"

struct SourcePair {
    s32 first;
    s32 second;
};

struct DestinationTriple {
    s32 second;
    s32 zero;
    s32 first;
};

extern struct SourcePair D_800A3190[];

void func_80069580(u32 index, struct DestinationTriple *destination)
{
    destination->zero = 0;
    destination->first = D_800A3190[index].first;
    destination->second = D_800A3190[index].second;
}
