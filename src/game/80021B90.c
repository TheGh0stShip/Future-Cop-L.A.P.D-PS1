#include "game/types.h"

struct SpuVoiceRequest {
    u32 voices;
    u32 mask;
    s16 left;
    s16 right;
    u8 unk0C[8];
    s16 pitch;
    u8 unk16[0x2A];
};

extern void func_80099368(struct SpuVoiceRequest *request);

void func_80021B90(void)
{
    struct SpuVoiceRequest request;

    request.voices = 0xFFFFFF;
    request.mask = 0x13;
    request.pitch = 0;
    request.left = 0;
    request.right = 0;
    func_80099368(&request);
}
