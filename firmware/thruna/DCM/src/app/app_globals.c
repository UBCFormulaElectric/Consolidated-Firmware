#include "app_globals.h"

Globals        dcm_globals;
Globals *const globals = &dcm_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config = config;
}
