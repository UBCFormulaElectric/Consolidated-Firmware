#include <stdlib.h>
#include <assert.h>
#include "App_Charger.h"

struct Charger
{
    void (*enable)(void);
    void (*disable)(void);
    bool (*is_connected)(void);
};

struct Charger *App_Charger_Create(
    void (*enable_charger)(void),
    void (*disable_charger)(void),
    bool (*is_charger_connected)(void))
{
    struct Charger *charger = malloc(sizeof(struct Charger));
    assert(charger != NULL);

    charger->enable       = enable_charger;
    charger->disable      = disable_charger;
    charger->is_connected = is_charger_connected;

    return charger;
}

void App_Charger_Destroy(struct Charger *charger)
{
    free(charger);
}

void App_Charger_Enable(struct Charger *charger)
{
    charger->enable();
}

void App_Charger_Disable(struct Charger *charger)
{
    charger->disable();
}

bool App_Charger_IsConnected(struct Charger *charger)
{
    return charger->is_connected();
}
