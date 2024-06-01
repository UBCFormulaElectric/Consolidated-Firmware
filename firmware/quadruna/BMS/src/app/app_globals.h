#pragma once

#include <stdint.h>
#include "io_faultLatch.h"
#include "io_thermistors.h"
#include "app_timer.h"

typedef struct
{
    const FaultLatch *bms_ok_latch;
    const FaultLatch *imd_ok_latch;
    const FaultLatch *bspd_ok_latch;
} GlobalsConfig;

typedef struct
{
    // Config.
    const GlobalsConfig *config;

    // State variables.
    uint32_t     cell_monitor_settle_count;
    uint32_t     num_precharge_failures;
    bool         precharge_limit_exceeded;
    bool         fault_encountered;
    bool         broadcast_charger_connected;
    TimerChannel precharge_lower_bound_timer;
    TimerChannel precharge_upper_bound_timer;
    uint32_t     ignore_charger_fault_counter;
    uint32_t     charger_exit_counter;
    uint32_t     charger_connected_counter;
} Globals;

extern Globals *const globals;

/**
 * Initialize the globals module.
 * @param config Globals config.
 */
void app_globals_init(const GlobalsConfig *config);
