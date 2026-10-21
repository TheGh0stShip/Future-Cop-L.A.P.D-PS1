struct LinkNode {
    unsigned char pad_00[0x48];
    struct LinkNode *back;
    unsigned char pad_4C[0x38];
    struct LinkNode *forward;
};

void func_8004AAC0(struct LinkNode *left, struct LinkNode *right)
{
    right->back = left->forward;
    left->forward = right;
}
