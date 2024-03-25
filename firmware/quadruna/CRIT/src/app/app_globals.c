#include "app_globals.h"

Globals        crit_globals;
Globals *const globals = &crit_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config = config;
}
