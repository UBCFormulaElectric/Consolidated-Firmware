#pragma once
#include <stdint.h>
#include <stdbool.h>

struct IO_RTC_CTRL1
{
    uint8_t CAP_SEL : 1;
    uint8_t T : 1;
    uint8_t STOP : 1;   // RTC time counter stopped or not
    uint8_t SR : 1;     // software reset
    uint8_t _12_24 : 1; // 12 or 24 hour mode
    uint8_t SIE : 1;    // Interrupt enable
    uint8_t AIE : 1;    // Alarm interrupt enable
    uint8_t CIE : 1;    // interrupt pulses are generated at every correction cycle
};

void io_rtc_program_ctrl1()