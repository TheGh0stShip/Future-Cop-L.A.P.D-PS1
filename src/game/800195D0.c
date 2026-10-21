struct LinkState {
    unsigned char pad_00[0xC];
    int count;
    int *link;
};

void func_800195D0(struct LinkState *state, int *link)
{
    *link = (int)state->link;
    state->link = link;
    state->count++;
}
