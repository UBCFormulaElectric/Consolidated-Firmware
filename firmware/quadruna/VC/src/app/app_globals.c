#include "app_globals.h"

Globals        vc_globals;
Globals *const globals = &vc_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config = config;
}