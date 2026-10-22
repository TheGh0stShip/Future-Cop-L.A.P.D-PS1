struct CallbackTable {
    void (*reserved_00)(void);
    void (*reserved_04)(void);
    void (*reserved_08)(void);
    void (*reserved_0C)(void);
    void (*reserved_10)(void);
    void (*callback)(int operation, void *argument);
};

extern struct CallbackTable *D_8009DD10;

void func_80091510(void *argument)
{
    D_8009DD10->callback(4, argument);
}
