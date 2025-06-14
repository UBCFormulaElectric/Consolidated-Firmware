#include <string.h>
#include "app_globals.h"

Globals        bms_globals;
Globals *const globals = &bms_globals;

void app_globals_init()
{
    memset(globals, 0U, sizeof(Globals));
    globals->cell_monitor_settle_count          = 0;
    globals->ignore_charger_fault_counter       = 0;
    globals->charger_exit_counter               = 0;
}
