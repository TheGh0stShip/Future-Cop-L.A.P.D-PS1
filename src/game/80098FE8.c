#include "game/types.h"

typedef struct {
    s16 left;
    s16 right;
} SpuVolume;

typedef struct {
    SpuVolume volume;
    s32 reverb;
    s32 mix;
} SpuExtAttr;

typedef struct {
    u32 mask;
    SpuVolume mvol;
    SpuVolume mvolmode;
    SpuVolume mvolx;
    SpuExtAttr cd;
    SpuExtAttr ext;
} SpuCommonAttr;

typedef struct {
    u8 pad00[0x180];
    u16 mainVolumeLeft;
    u16 mainVolumeRight;
    u8 pad184[0x26];
    u16 control;
    u8 pad1AC[4];
    u16 cdVolumeLeft;
    u16 cdVolumeRight;
    u16 extVolumeLeft;
    u16 extVolumeRight;
} SpuRegisters;

extern SpuRegisters *D_8009EEA4;

void func_80098FE8(SpuCommonAttr *attr)
{
    s32 mask;
    s32 setAll;
    s32 leftVolume;
    s32 rightVolume;
    s32 mode;
    s16 signedVolume;
    s32 unsignedVolume;

    leftVolume = 0;
    __asm__("" : "=r"(leftVolume) : "0"(leftVolume));
    mask = attr->mask;
    setAll = mask == 0;
    rightVolume = 0;

    if (setAll || (mask & 1)) {
        if (setAll || (mask & 4)) {
            switch (attr->mvolmode.left) {
            case 1: mode = 0x8000; break;
            case 2: mode = 0x9000; break;
            case 3: mode = 0xA000; break;
            case 4: mode = 0xB000; break;
            case 5: mode = 0xC000; break;
            case 6: mode = 0xD000; break;
            case 7: mode = 0xE000; break;
            case 0:
                leftVolume = (u16)attr->mvol.left;
                mode = 0;
                break;
            default:
                leftVolume = (u16)attr->mvol.left;
                mode = 0;
                break;
            }
        } else {
            leftVolume = (u16)attr->mvol.left;
            mode = 0;
        }
        if (mode != 0) {
            unsignedVolume = (u16)attr->mvol.left;
            signedVolume = attr->mvol.left;
            if (signedVolume >= 0x80) {
                leftVolume = 0x7F;
            } else if (signedVolume >= 0) {
                leftVolume = unsignedVolume;
            } else {
                leftVolume = 0;
            }
        }
        D_8009EEA4->mainVolumeLeft = (leftVolume & 0x7FFF) | mode;
    }

    if (setAll || (mask & 2)) {
        if (setAll || (mask & 8)) {
            switch (attr->mvolmode.right) {
            case 1: mode = 0x8000; break;
            case 2: mode = 0x9000; break;
            case 3: mode = 0xA000; break;
            case 4: mode = 0xB000; break;
            case 5: mode = 0xC000; break;
            case 6: mode = 0xD000; break;
            case 7: mode = 0xE000; break;
            case 0:
                rightVolume = (u16)attr->mvol.right;
                mode = 0;
                break;
            default:
                rightVolume = (u16)attr->mvol.right;
                mode = 0;
                break;
            }
        } else {
            rightVolume = (u16)attr->mvol.right;
            mode = 0;
        }
        if (mode != 0) {
            unsignedVolume = (u16)attr->mvol.right;
            signedVolume = attr->mvol.right;
            if (signedVolume >= 0x80) {
                rightVolume = 0x7F;
            } else if (signedVolume >= 0) {
                rightVolume = unsignedVolume;
            } else {
                rightVolume = 0;
            }
        }
        D_8009EEA4->mainVolumeRight = (rightVolume & 0x7FFF) | mode;
    }

    if (setAll || (mask & 0x40)) {
        D_8009EEA4->cdVolumeLeft = attr->cd.volume.left;
    }
    if (setAll || (mask & 0x80)) {
        D_8009EEA4->cdVolumeRight = attr->cd.volume.right;
    }
    if (setAll || (mask & 0x400)) {
        D_8009EEA4->extVolumeLeft = attr->ext.volume.left;
    }
    if (setAll || (mask & 0x800)) {
        D_8009EEA4->extVolumeRight = attr->ext.volume.right;
    }

    if (setAll || (mask & 0x100)) {
        if (attr->cd.reverb == 0) {
            D_8009EEA4->control &= ~4;
        } else {
            D_8009EEA4->control |= 4;
        }
    }
    if (setAll || (mask & 0x200)) {
        if (attr->cd.mix == 0) {
            D_8009EEA4->control &= ~1;
        } else {
            D_8009EEA4->control |= 1;
        }
    }
    if (setAll || (mask & 0x1000)) {
        if (attr->ext.reverb == 0) {
            D_8009EEA4->control &= ~8;
        } else {
            D_8009EEA4->control |= 8;
        }
    }
    if (setAll || (mask & 0x2000)) {
        if (attr->ext.mix == 0) {
            D_8009EEA4->control &= ~2;
        } else {
            D_8009EEA4->control |= 2;
        }
    }
    __asm__ volatile("");
}
