#include "app_globals.h"

Globals        dim_globals;
Globals *const globals = &dim_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config              = config;
}
