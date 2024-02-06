#include "app_globals.h"

Globals        dim_globals;
Globals *const globals = &dim_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config              = config;
    uint32_t warining_array[64]   = { 0 };
    uint32_t fault_array[64]      = { 0 };
    uint32_t total_faults_num[64] = { 0 };
    globals->total_faults        = total_faults_num;
    globals->fault_code_array    = fault_array;
    globals->warning_code_array  = warining_array;
    globals->warning_element_num = 0;
    globals->fault_element_num   = 0;
    globals->element_num         = 0;
    globals->total_element_num   = 0;
}
