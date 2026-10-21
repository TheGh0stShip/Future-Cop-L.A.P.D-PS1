struct ListNode {
    struct ListNode *previous;
    struct ListNode *next;
    struct ListNode *tail;
};

struct ListNode *func_8003BEF0(struct ListNode *node)
{
    struct ListNode *tail = node->tail;

    if (tail != 0) {
        node->tail = tail->previous;
        tail->previous = node->next;
        node->next = tail;
    }
    return tail;
}
