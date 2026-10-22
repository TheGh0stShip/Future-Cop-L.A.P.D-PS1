#include "game/types.h"

struct Unk17994Block {
    s32 words[12];
};

struct Unk17994Node {
    u8 unk00[0x28];
    struct Unk17994Block first;
    struct Unk17994Block second;
    s32 id;
    struct Unk17994Node *next;
    struct Unk17994Node *previous;
};

extern struct Unk17994Block D_8009AA38;
extern struct Unk17994Node *D_8009F588;
extern s32 D_8009F58C;

extern void *func_80017EBC(s32 size, s32 tag);

s32 func_80017994(void)
{
    register struct Unk17994Node *node __asm__("$5");
    register struct Unk17994Node *tail __asm__("$6");
    register struct Unk17994Block *template __asm__("$3");
    register s32 firstWord __asm__("$4");

    node = func_80017EBC(0x94, 2);
    template = &D_8009AA38;
    firstWord = template->words[0];
    node->first.words[0] = firstWord;
    node->first.words[1] = template->words[1];
    node->first.words[2] = template->words[2];
    node->first.words[3] = template->words[3];
    node->first.words[4] = template->words[4];
    node->first.words[5] = template->words[5];
    node->first.words[6] = template->words[6];
    node->first.words[7] = template->words[7];
    node->first.words[8] = template->words[8];
    node->first.words[9] = template->words[9];
    node->first.words[10] = template->words[10];
    node->first.words[11] = template->words[11];
    node->second.words[0] = firstWord;
    node->second.words[1] = template->words[1];
    node->second.words[2] = template->words[2];
    node->second.words[3] = template->words[3];
    node->second.words[4] = template->words[4];
    node->second.words[5] = template->words[5];
    node->second.words[6] = template->words[6];
    node->second.words[7] = template->words[7];
    node->second.words[8] = template->words[8];
    node->second.words[9] = template->words[9];
    node->second.words[10] = template->words[10];
    node->second.words[11] = template->words[11];
    node->id = D_8009F58C++;
    node->next = 0;
    node->previous = 0;

    if (D_8009F588 != 0) {
        tail = D_8009F588;
        while (tail->next != 0) {
            tail = tail->next;
        }
        tail->next = node;
        node->previous = tail;
    } else {
        D_8009F588 = node;
    }
    return node->id;
}
