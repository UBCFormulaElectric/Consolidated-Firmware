#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

// https://datasheet.ciiva.com/pdfs/VipMasterIC/IC/PHGL/PHGL-S-A0000776674/PHGL-S-A0000776674-1.pdf?src-supplier=IHS+Markit

// standard digital format, not BCD.
// need to be converted to BCD format if needed.

// only 24-hour mode is supported
typedef struct
{
    uint8_t seconds;  // 0-59
    uint8_t minutes;  // 0-59
    uint8_t hours;    // 0-23 in 24-hour
    uint8_t day;      // 1-31
    uint8_t weekdays; // Weekday (0d-6, Sunday=0)
    uint8_t month;    // Month (1d-12)
    uint8_t year;     // Year (0d-99) need to offset by 2000 when you interpret it
} IoRtcTime;

// always 24-hour mode

// any initialization setup for the RTC chip
// if io error occurs, return false
ExitCode io_rtc_init(void);

// set the time on the RTC chip
// if io error occurs, return false
ExitCode io_rtc_setTime(IoRtcTime *time);

// read the time from the RTC chip
// if io error occurs, return false
ExitCode io_rtc_readTime(IoRtcTime *time);

// check the health of the RTC chip
// struct IoRtcHealth io_rtc_healthCheck(void);

// reset the RTC chip
void io_rtc_reset(void);