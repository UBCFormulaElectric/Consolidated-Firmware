#include <stdlib.h>
#include <assert.h>
#include "App_Charger.h"

struct Charger
{
    void (*enable)(void);
    void (*disable)(void);
    bool (*is_connected)(void);
    bool (*has_charger_faulted)(void);
    bool is_enabled;
};

struct Charger *App_Charger_Create(
    void (*enable_charger)(void),
    void (*disable_charger)(void),
    bool (*is_charger_connected)(void),
    bool (*has_charger_faulted)(void))
{
    struct Charger *charger = malloc(sizeof(struct Charger));
    assert(charger != NULL);

    charger->enable              = enable_charger;
    charger->disable             = disable_charger;
    charger->is_connected        = is_charger_connected;
    charger->has_charger_faulted = has_charger_faulted;
    charger->is_enabled          = false;

    return charger;
}

void App_Charger_Destroy(struct Charger *charger)
{
    free(charger);
}

void App_Charger_Enable(struct Charger *charger)
{
    charger->is_enabled = true;
    charger->enable();
}

void App_Charger_Disable(struct Charger *charger)
{
    charger->is_enabled = false;
    charger->disable();
}

bool App_Charger_IsConnected(struct Charger *charger)
{
    return charger->is_connected();
}

bool App_Charger_IsEnabled(const struct Charger *charger)
{
    return charger->is_enabled;
}

bool App_Charger_HasFaulted(const struct Charger *charger)
{
    return charger->has_charger_faulted();
}
