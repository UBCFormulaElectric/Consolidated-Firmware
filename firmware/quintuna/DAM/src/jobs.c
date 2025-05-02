#include "jobs.h"
#include "io_buzzer.h"
#include "io_rtc.h"
#include "io_log.h"

// static void jsoncan_transmit_func(const JsonCanMsg *tx_msg) {}

void jobs_init()
{
    io_rtc_init();
}

void jobs_run1Hz_tick(void) {}

void jobs_run100Hz_tick(void) {}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void) {}