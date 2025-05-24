#include "jobs.h"
#include "app_powerMonitoring.h"

void jobs_init() {}

void jobs_run1Hz_tick(void) {}

void jobs_run100Hz_tick(void) {}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void) {}

void jobs_runPowerMonitoring_tick(void)
{
    app_powerMonitoring_update();
}
