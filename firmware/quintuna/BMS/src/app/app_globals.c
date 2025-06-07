#include <string.h>
#include "app_globals.h"
// #include "states/app_prechargeState.h"

Globals        bms_globals;
Globals *const globals = &bms_globals;

void app_globals_init()
{
    memset(globals, 0U, sizeof(Globals));
    // TODO: Uncomment when precharge state is added
    // app_timer_init(&globals->precharge_lower_bound_timer, PRECHARGE_COMPLETION_LOWER_BOUND);
    // app_timer_init(&globals->precharge_upper_bound_timer, PRECHARGE_COMPLETION_UPPER_BOUND);
    globals->charger_connected_counter          = 0;
    globals->cell_monitor_settle_count          = 0;
    globals->ignore_charger_fault_counter       = 0;
    globals->charger_exit_counter               = 0;
    globals->precharge_limit_exceeded           = false;
    globals->broadcast_charger_connected        = true;
    globals->disable_charger_connected_hb_check = false;
}
