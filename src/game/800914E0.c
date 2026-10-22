struct CallbackTable {
    void (*reserved_00)(void);
    void (*callback)(void);
};

extern struct CallbackTable *D_8009DD10;

void func_800914E0(void)
{
    D_8009DD10->callback();
}
