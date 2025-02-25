#include "jobs.h"

// static void jsoncan_transmit_func(const JsonCanMsg *tx_msg) {}

void jobs_init() {}

void jobs_run1Hz_tick(void)
{
    // VERY IMPORTANT that allstates after state machine
    // this is because there are fault overrides in allStates
}

void jobs_run100Hz_tick(void)
{
    // VERY IMPORTANT that allstates after state machine
}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void) {}