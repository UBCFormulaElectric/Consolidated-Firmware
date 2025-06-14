#pragma once

#include <stdint.h>
#include "app_timer.h"

typedef struct
{
    // State variables.
    uint32_t cell_monitor_settle_count;
    uint32_t ignore_charger_fault_counter;
    uint32_t charger_exit_counter;
} Globals;

extern Globals *const globals;

/**
 * Initialize the globals module.
 * @param config Globals config.
 */
void app_globals_init();
