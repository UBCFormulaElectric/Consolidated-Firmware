#include "jobs.h"
#include "io_rtc.h"

// static void jsoncan_transmit_func(const JsonCanMsg *tx_msg) {}

void jobs_init() {
    
    io_rtc_init();

    struct IoRtcTime time = {
        .seconds = 0,
        .hours = 16,
        .minutes = 6,
        .weekdays = 6,
        .day = 22,
        .month = 3,
        .year = 2025
    }
    
}

void jobs_run1Hz_tick(void) {}

void jobs_run100Hz_tick(void) {}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void) {}