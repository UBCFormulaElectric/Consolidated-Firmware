#include <stdlib.h>
#include <assert.h>
#include "App_Charger.h"

struct Charger
{
    void (*enable)(void);
    void (*disable)(void);
    bool (*is_connected)(void);
    bool (*has_charger_faulted)(void);
    bool     is_enabled;
    uint16_t ignore_chgr_fault_counter;
    uint16_t chrg_exit_counter;
};

struct Charger *App_Charger_Create(
    void (*enable_charger)(void),
    void (*disable_charger)(void),
    bool (*is_charger_connected)(void),
    bool (*has_charger_faulted)(void))
{
    struct Charger *charger = malloc(sizeof(struct Charger));
    assert(charger != NULL);

    charger->enable                    = enable_charger;
    charger->disable                   = disable_charger;
    charger->is_connected              = is_charger_connected;
    charger->has_charger_faulted       = has_charger_faulted;
    charger->is_enabled                = false;
    charger->ignore_chgr_fault_counter = 0U;
    charger->chrg_exit_counter         = 0U;

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

void App_Charger_IncrementFaultCounterVal(struct Charger *charger)
{
    charger->ignore_chgr_fault_counter++;
}

uint16_t App_Charger_GetFaultCounterVal(const struct Charger *charger)
{
    return charger->ignore_chgr_fault_counter;
}

void App_Charger_ResetFaultCounterVal(struct Charger *charger)
{
    charger->ignore_chgr_fault_counter = 0U;
}

void App_Charger_IncrementExitCounterVal(struct Charger *charger)
{
    charger->chrg_exit_counter++;
}

uint16_t App_Charger_GetExitCounterVal(const struct Charger *charger)
{
    return charger->chrg_exit_counter;
}

void App_Charger_ResetExitCounterVal(struct Charger *charger)
{
    charger->chrg_exit_counter = 0U;
}
