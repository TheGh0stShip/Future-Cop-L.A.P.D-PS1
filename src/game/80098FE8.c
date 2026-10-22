#include "game/types.h"

typedef struct {
    u16 left;
    u16 right;
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
    SpuVolume mainVolume;
    u8 pad184[0x26];
    u16 control;
    u8 pad1AC[4];
    SpuVolume cdVolume;
    SpuVolume extVolume;
} SpuRegisters;

extern SpuRegisters *D_8009EEA4;

void func_80098FE8(SpuCommonAttr *attr)
{
    u16 leftMode;
    u16 rightMode;
    u16 leftVolume;
    u16 rightVolume;
    u32 mask;
    s32 setAll;
    u16 control;

    leftVolume = 0;
    rightVolume = 0;
    mask = attr->mask;
    setAll = mask == 0;

    if (setAll || (mask & 1)) {
        if (setAll || (mask & 4)) {
            switch ((s16)attr->mvolmode.left) {
            case 1: leftMode = 0x8000; break;
            case 2: leftMode = 0x9000; break;
            case 3: leftMode = 0xA000; break;
            case 4: leftMode = 0xB000; break;
            case 5: leftMode = 0xC000; break;
            case 6: leftMode = 0xD000; break;
            case 7: leftMode = 0xE000; break;
            case 0:
            default:
                leftVolume = attr->mvol.left;
                leftMode = 0;
                break;
            }
        } else {
            leftVolume = attr->mvol.left;
            leftMode = 0;
        }
        if (leftMode != 0) {
            if ((s16)attr->mvol.left >= 0x80) {
                leftVolume = 0x7F;
            } else if ((s16)attr->mvol.left < 0) {
                leftVolume = 0;
            } else {
                leftVolume = attr->mvol.left;
            }
        }
        D_8009EEA4->mainVolume.left = (leftVolume & 0x7FFF) | leftMode;
    }

    if (setAll || (mask & 2)) {
        if (setAll || (mask & 8)) {
            switch ((s16)attr->mvolmode.right) {
            case 1: rightMode = 0x8000; break;
            case 2: rightMode = 0x9000; break;
            case 3: rightMode = 0xA000; break;
            case 4: rightMode = 0xB000; break;
            case 5: rightMode = 0xC000; break;
            case 6: rightMode = 0xD000; break;
            case 7: rightMode = 0xE000; break;
            case 0:
            default:
                rightVolume = attr->mvol.right;
                rightMode = 0;
                break;
            }
        } else {
            rightVolume = attr->mvol.right;
            rightMode = 0;
        }
        if (rightMode != 0) {
            if ((s16)attr->mvol.right >= 0x80) {
                rightVolume = 0x7F;
            } else if ((s16)attr->mvol.right < 0) {
                rightVolume = 0;
            } else {
                rightVolume = attr->mvol.right;
            }
        }
        D_8009EEA4->mainVolume.right = (rightVolume & 0x7FFF) | rightMode;
    }

    if (setAll || (mask & 0x40)) {
        D_8009EEA4->cdVolume.left = attr->cd.volume.left;
    }
    if (setAll || (mask & 0x80)) {
        D_8009EEA4->cdVolume.right = attr->cd.volume.right;
    }
    if (setAll || (mask & 0x400)) {
        D_8009EEA4->extVolume.left = attr->ext.volume.left;
    }
    if (setAll || (mask & 0x800)) {
        D_8009EEA4->extVolume.right = attr->ext.volume.right;
    }

    if (setAll || (mask & 0x100)) {
        if (attr->cd.reverb == 0) {
            control = D_8009EEA4->control;
            control &= ~4;
            D_8009EEA4->control = control;
        } else {
            control = D_8009EEA4->control;
            control |= 4;
            D_8009EEA4->control = control;
        }
    }
    if (setAll || (mask & 0x200)) {
        if (attr->cd.mix == 0) {
            control = D_8009EEA4->control;
            control &= ~1;
            D_8009EEA4->control = control;
        } else {
            control = D_8009EEA4->control;
            control |= 1;
            D_8009EEA4->control = control;
        }
    }
    if (setAll || (mask & 0x1000)) {
        if (attr->ext.reverb == 0) {
            control = D_8009EEA4->control;
            control &= ~8;
            D_8009EEA4->control = control;
        } else {
            control = D_8009EEA4->control;
            control |= 8;
            D_8009EEA4->control = control;
        }
    }
    if (setAll || (mask & 0x2000)) {
        if (attr->ext.mix == 0) {
            control = D_8009EEA4->control;
            control &= ~2;
            D_8009EEA4->control = control;
        } else {
            control = D_8009EEA4->control;
            control |= 2;
            D_8009EEA4->control = control;
        }
    }
}
