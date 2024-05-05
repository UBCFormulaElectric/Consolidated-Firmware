#include "app_globals.h"

Globals        rsm_globals;
Globals *const globals = &rsm_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config = config;
}
