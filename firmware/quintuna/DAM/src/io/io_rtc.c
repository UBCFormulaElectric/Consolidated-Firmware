#include <stdint.h>
#include "hw_i2c.h"
#include "hw_i2cs.h"

#define REG_CONTROL_1 0x00     // Control and status settings
#define REG_CONTROL_2 0x01     // Alarm and watchdog control
#define REG_CONTROL_3 0x02     // Battery switch-over function
#define REG_SECONDS 0x03       // Seconds register (0–59)
#define REG_MINUTES 0x04       // Minutes register (0–59)
#define REG_HOURS 0x05         // Hours register (0–23 or 1–12)
#define REG_DAYS 0x06          // Days register (1–31)
#define REG_WEEKDAYS 0x07      // Weekdays register (0–6, Monday=0)
#define REG_MONTHS 0x08        // Months register (1–12)
#define REG_YEARS 0x09         // Years register (0–99)
#define REG_MINUTE_ALARM 0x0A  // Minute alarm (0–59)
#define REG_HOUR_ALARM 0x0B    // Hour alarm (0–23 or 1–12)
#define REG_DAY_ALARM 0x0C     // Day alarm (1–31)
#define REG_WEEKDAY_ALARM 0x0D // Weekday alarm (0–6)
#define REG_OFFSET 0x0E        // Offset calibration register
#define REG_CLOCKOUT 0x0F      // CLKOUT control register
#define REG_TIMER_CONTROL 0x10 // Timer control register
#define REG_TIMER_A_FREQ 0x11  // Timer A frequency register
#define REG_TIMER_A_VALUE 0x12 // Timer A value register
#define REG_TIMER_B_FREQ 0x13  // Timer B frequency register
#define REG_TIMER_B_VALUE 0x14 // Timer B value register

/**
 * Control Register 1 (REG_CONTROL_1)
 * - Manages oscillator start/stop, time format, and interrupt enable bits.
 */
typedef struct __attribute__((packed))
{
    uint8_t CIE : 1;     // 0: Disable clock output interrupt, 1: Enable
    uint8_t AIE : 1;     // 0: Disable alarm interrupt, 1: Enable
    uint8_t SIE : 1;     // 0: Disable seconds interrupt, 1: Enable
    uint8_t _12_24 : 1;  // 0: 24-hour mode, 1: 12-hour mode
    uint8_t SR : 1;      // 0: Normal mode, 1: Software reset
    uint8_t STOP : 1;    // 0: Clock runs, 1: Clock stops
    uint8_t T : 1;       // Must always be written as 0
    uint8_t CAP_SEL : 1; // Oscillator capacitor selection
} Control1_t;

/**
 * Control Register 2 (REG_CONTROL_2)
 * - Controls alarms and watchdog timer functions.
 */
typedef struct __attribute__((packed))
{
    uint8_t CTBIE : 1; // Countdown timer interrupt enable
    uint8_t CTAIE : 1; // Countdown timer A interrupt enable
    uint8_t WTAIE : 1; // Watchdog timer alarm interrupt enable
    uint8_t AF : 1;    // Alarm flag (cleared by writing 0)
    uint8_t SF : 1;    // Seconds flag (cleared by writing 0)
    uint8_t CTBF : 1;  // Countdown timer B flag
    uint8_t CTAF : 1;  // Countdown timer A flag
    uint8_t WTAF : 1;  // Watchdog timer alarm flag
} Control2_t;

/**
 * Control Register 3 (REG_CONTROL_3)
 * - Configures battery switch-over and power management.
 */
typedef struct __attribute__((packed))
{
    uint8_t BLIE : 1; // Battery low interrupt enable
    uint8_t BSIE : 1; // Battery switch interrupt enable
    uint8_t BLF : 1;  // Battery low flag
    uint8_t BSF : 1;  // Battery switch flag
    uint8_t PM : 3;   // Power management mode
    uint8_t RESERVED : 1;
} Control3_t;

/**
 * Seconds Register (REG_SECONDS)
 * - Stores the seconds (0–59).
 */
typedef struct __attribute__((packed))
{
    uint8_t SECONDS : 6; // Seconds (0–59)
    uint8_t OS : 1;      // Oscillator stop flag (1: stopped, 0: running)
    uint8_t RESERVED : 1;
} Seconds_t;

/**
 * Minutes Register (REG_MINUTES)
 * - Stores the minutes (0–59).
 */
typedef struct __attribute__((packed))
{
    uint8_t MINUTES : 7; // Minutes (0–59)
    uint8_t RESERVED : 1;
} Minutes_t;

/**
 * Hours Register (REG_HOURS)
 * - Stores the hours (0–23 in 24-hour mode, 1–12 in 12-hour mode).
 */
typedef struct __attribute__((packed))
{
    uint8_t HOURS : 5; // Hours (0–23 or 1–12)
    uint8_t AMPM : 1;  // 0: AM, 1: PM (only in 12-hour mode)
    uint8_t RESERVED : 2;
} Hours_t;

/**
 * Days Register (REG_DAYS)
 * - Stores the day of the month (1–31).
 */
typedef struct __attribute__((packed))
{
    uint8_t DAYS : 6; // Days (1–31)
    uint8_t RESERVED : 2;
} Days_t;

/**
 * Weekdays Register (REG_WEEKDAYS)
 * - Stores the weekday (0–6).
 */
typedef struct __attribute__((packed))
{
    uint8_t WEEKDAYS : 3; // Weekday (0–6, Monday=0)
    uint8_t RESERVED : 5;
} Weekdays_t;

/**
 * Months Register (REG_MONTHS)
 * - Stores the month (1–12).
 */
typedef struct __attribute__((packed))
{
    uint8_t MONTHS : 4; // Months (1–12)
    uint8_t RESERVED : 4;
} Months_t;

/**
 * Years Register (REG_YEARS)
 * - Stores the year (0–99).
 */
typedef struct __attribute__((packed))
{
    uint8_t YEARS : 7; // Years (0–99)
    uint8_t RESERVED : 1;
} Years_t;

/**
 * Minute Alarm Register (REG_MINUTE_ALARM)
 * - Stores the minute alarm setting (0–59).
 */
typedef struct __attribute__((packed))
{
    uint8_t MINUTE_ALARM : 6; // Minute alarm (0–59)
    uint8_t AEN_M : 1;        // Alarm enable (0: disabled, 1: enabled)
    uint8_t RESERVED : 1;
} MinuteAlarm_t;

/**
 * Hour Alarm Register (REG_HOUR_ALARM)
 * - Stores the hour alarm setting (0–23 or 1–12).
 */
typedef struct __attribute__((packed))
{
    uint8_t HOUR_ALARM : 5; // Hour alarm (0–23 or 1–12)
    uint8_t AMPM : 1;       // AM/PM flag for 12-hour mode
    uint8_t AEN_H : 1;      // Alarm enable (0: disabled, 1: enabled)
    uint8_t RESERVED : 1;
} HourAlarm_t;

/**
 * Offset Register (REG_OFFSET)
 * - Used for temperature compensation.
 */
typedef struct __attribute__((packed))
{
    uint8_t OFFSET : 7; // Offset value for drift correction
    uint8_t MODE : 1;   // Offset mode selection
} Offset_t;

/**
 * CLKOUT Control Register (REG_CLOCKOUT)
 * - Configures the clock output frequency.
 */
typedef struct __attribute__((packed))
{
    uint8_t COF : 3; // Clock output frequency selection
    uint8_t RESERVED : 5;
} ClockOut_t;

/**
 * Timer Control Register (REG_TIMER_CONTROL)
 * - Controls the operation of timers A and B.
 */
typedef struct __attribute__((packed))
{
    uint8_t TE_A : 1;  // Timer A enable (1 = enabled)
    uint8_t TE_B : 1;  // Timer B enable (1 = enabled)
    uint8_t TI_TP : 1; // Timer interrupt/pulse mode (1 = interrupt, 0 = pulse)
    uint8_t TD_A : 2;  // Timer A clock source
    uint8_t TD_B : 2;  // Timer B clock source
    uint8_t RESERVED : 1;
} TimerControl_t;

/**
 * Timer A Frequency Register (REG_TIMER_A_FREQ)
 * - Configures the frequency source for Timer A.
 */
typedef struct __attribute__((packed))
{
    uint8_t TAF : 4; // Timer A frequency selection
    uint8_t RESERVED : 4;
} TimerAFreq_t;

/**
 * Timer A Value Register (REG_TIMER_A_VALUE)
 * - Holds the countdown value for Timer A.
 */
typedef struct __attribute__((packed))
{
    uint8_t TAV; // Timer A countdown value
} TimerAValue_t;

/**
 * Timer B Frequency Register (REG_TIMER_B_FREQ)
 * - Configures the frequency source for Timer B.
 */
typedef struct __attribute__((packed))
{
    uint8_t TBF : 4; // Timer B frequency selection
    uint8_t RESERVED : 4;
} TimerBFreq_t;

/**
 * Timer B Value Register (REG_TIMER_B_VALUE)
 * - Holds the countdown value for Timer B.
 */
typedef struct __attribute__((packed))
{
    uint8_t TBV; // Timer B countdown value
} TimerBValue_t;

void io_rtc_init(void)
{
    Control1_t control1 = {
        .CIE     = 0,
        .AIE     = 0,
        .SIE     = 0,
        ._12_24  = 0,
        .SR      = 0,
        .STOP    = 0,
        .T       = 0,
        .CAP_SEL = 0,
    };
}