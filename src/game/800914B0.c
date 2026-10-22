struct CallbackTable {
    void (*reserved_00)(void);
    void (*reserved_04)(void);
    void (*callback)(void);
};

extern struct CallbackTable *D_8009DD10;

void func_800914B0(void)
{
    D_8009DD10->callback();
}
