#include "app_globals.h"

Globals        dim_globals;
Globals *const globals = &dim_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config                   = config;
    uint8_t point_element             = 0;
    globals->element_num              = &point_element;
    uint8_t array[64]                 = { 0 };
    globals->fault_warning_code_array = array;
}
