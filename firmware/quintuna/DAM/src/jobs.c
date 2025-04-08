#include "jobs.h"
#include "io_buzzer.h"
#include "io_rtc.h"
#include "io_log.h"

// static void jsoncan_transmit_func(const JsonCanMsg *tx_msg) {}

void jobs_init()
{
    io_rtc_init();

    IoRtcTime time = {
        .seconds = 50,
        .minutes = 16,
        .hours   = 19,
        .day     = 7,
        .month   = 4,
        .year    = 25, // 2023
    };
}

void jobs_run1Hz_tick(void) {}

void jobs_run100Hz_tick(void) {}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void) {}