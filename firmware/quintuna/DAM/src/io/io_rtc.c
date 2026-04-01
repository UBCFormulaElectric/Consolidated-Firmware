#include <stdint.h>
#include "hw_i2cs.h"
#include "io_rtc.h"
#include "io_log.h"

// https://datasheet.ciiva.com/pdfs/VipMasterIC/IC/PHGL/PHGL-S-A0000776674/PHGL-S-A0000776674-1.pdf?src-supplier=IHS+Markit

// standard digital format, not BCD.
// need to be converted to BCD format if needed.

#define RTC_I2C_ADDR 0x68
#define REG_CONTROL_1 0x00     // Control and status settings
#define REG_CONTROL_2 0x01     // Alarm and watchdog control
#define REG_CONTROL_3 0x02     // Battery switch-over function
#define REG_SECONDS 0x03       // Seconds register (0d-59)
#define REG_MINUTES 0x04       // Minutes register (0d-59)
#define REG_HOURS 0x05         // Hours register (0d-23 or 1d-12)
#define REG_DAYS 0x06          // Days register (1d-31)
#define REG_WEEKDAYS 0x07      // Weekdays register (0d-6, Monday=0)
#define REG_MONTHS 0x08        // Months register (1d-12)
#define REG_YEARS 0x09         // Years register (0d-99)
#define REG_MINUTE_ALARM 0x0A  // Minute alarm (0d-59)
#define REG_HOUR_ALARM 0x0B    // Hour alarm (0d-23 or 1d-12)
#define REG_DAY_ALARM 0x0C     // Day alarm (1d-31)
#define REG_WEEKDAY_ALARM 0x0D // Weekday alarm (0d-6)
#define REG_OFFSET 0x0E        // Offset calibration register
#define REG_CLOCKOUT 0x0F      // CLKOUT control register
#define REG_TIMER_CONTROL 0x10 // Timer control register
#define REG_TIMER_A_FREQ 0x11  // Timer A frequency register
#define REG_TIMER_A_VALUE 0x12 // Timer A value register
#define REG_TIMER_B_FREQ 0x13  // Timer B frequency register
#define REG_TIMER_B_VALUE 0x14 // Timer B value register

#pragma GCC diagnostic ignored "-Wconversion"

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
 * - Stores the seconds (0d-59).
 */
typedef struct __attribute__((packed))
{
    uint8_t SECONDS : 7; // Seconds (0d-59)
    uint8_t OS : 1;      // Oscillator stop flag (1: stopped, 0: running)
} Seconds_t;

/**
 * Minutes Register (REG_MINUTES)
 * - Stores the minutes (0d-59).
 */
typedef struct __attribute__((packed))
{
    uint8_t MINUTES : 7; // Minutes (0d-59) BCD format
    uint8_t RESERVED : 1;
} Minutes_t;

/**
 * Hours Register (REG_HOURS)
 * - Stores the hours (0d-23 in 24-hour mode, 1d-12 in 12-hour mode).
 */
typedef struct __attribute__((packed))
{
    uint8_t HOURS : 6; // Hours (0d-23 or 1d-12)
    uint8_t RESERVED : 2;
} Hours_t; // 24 hour mode

/**
 * Days Register (REG_DAYS)
 * - Stores the day of the month (1d-31).
 */
typedef struct __attribute__((packed))
{
    uint8_t DAYS : 6; // Days BCD format (1d-31)
    uint8_t RESERVED : 2;
} Days_t;

/**
 * Weekdays Register (REG_WEEKDAYS)
 * - Stores the weekday (0d-6).
 */
typedef struct __attribute__((packed))
{
    uint8_t WEEKDAYS : 3; // Weekday (0d-6, Sunday=0)
    uint8_t RESERVED : 5;
} Weekdays_t;

/**
 * Months Register (REG_MONTHS)
 * - Stores the month (1d-12).
 */
typedef struct __attribute__((packed))
{
    uint8_t MONTHS : 4; // Months (1d-12) BCD format
    uint8_t RESERVED : 4;
} Months_t;

/**
 * Years Register (REG_YEARS)
 * - Stores the year (0d-99).
 */
typedef struct __attribute__((packed))
{
    uint8_t YEARS : 7; // Years (0d-99) BCD format
    uint8_t RESERVED : 1;
} Years_t;

/**
 * Minute Alarm Register (REG_MINUTE_ALARM)
 * - Stores the minute alarm setting (0d-59).
 */
typedef struct __attribute__((packed))
{
    uint8_t MINUTE_ALARM : 7; // Minute alarm (0d-59)
    uint8_t AEN_M : 1;        // Alarm enable (0: disabled, 1: enabled)
} MinuteAlarm_t;

/**
 * Hour Alarm Register (REG_HOUR_ALARM)
 * - Stores the hour alarm setting (0d-23 or 1d-12).
 */
typedef struct __attribute__((packed))
{
    uint8_t HOUR_ALARM : 6; //  to 23 in 24 hour mode)
    uint8_t RESERVED : 1;
    uint8_t AEN_H : 1;
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
    uint8_t TBC : 1;
    uint8_t TAC : 2;
    uint8_t COF : 3;
    uint8_t TBM : 1;
    uint8_t TAM : 1;
} ClockOut_t;

/**
 * Timer A Frequency Register (REG_TIMER_A_FREQ)
 * - Configures the frequency source for Timer A.
 */
typedef struct __attribute__((packed))
{
    uint8_t TAQ : 3; // Timer A frequency selection
    uint8_t RESERVED : 5;
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
    uint8_t TBQ : 3; // Timer B frequency selection
    uint8_t RESERVED : 1;
    uint8_t TBW : 3;
    uint8_t RESERVED1 : 1;
} TimerBFreq_t;

/**
 * Timer B Value Register (REG_TIMER_B_VALUE)
 * - Holds the countdown value for Timer B.
 */
typedef struct __attribute__((packed))
{
    uint8_t TBV; // Timer B countdown value
} TimerBValue_t;

_Static_assert(sizeof(Control1_t) == 1, "Control1_t must be 1 byte");
_Static_assert(sizeof(Control2_t) == 1, "Control2_t must be 1 byte");
_Static_assert(sizeof(Control3_t) == 1, "Control3_t must be 1 byte");
_Static_assert(sizeof(Seconds_t) == 1, "Seconds_t must be 1 byte");
_Static_assert(sizeof(Minutes_t) == 1, "Minutes_t must be 1 byte");
_Static_assert(sizeof(Hours_t) == 1, "Hours_t must be 1 byte");
_Static_assert(sizeof(Days_t) == 1, "Days_t must be 1 byte");
_Static_assert(sizeof(Weekdays_t) == 1, "Weekdays_t must be 1 byte");
_Static_assert(sizeof(Months_t) == 1, "Months_t must be 1 byte");
_Static_assert(sizeof(Years_t) == 1, "Years_t must be 1 byte");
_Static_assert(sizeof(MinuteAlarm_t) == 1, "MinuteAlarm_t must be 1 byte");
_Static_assert(sizeof(HourAlarm_t) == 1, "HourAlarm_t must be 1 byte");
_Static_assert(sizeof(Offset_t) == 1, "Offset_t must be 1 byte");
_Static_assert(sizeof(ClockOut_t) == 1, "ClockOut_t must be 1 byte");
_Static_assert(sizeof(TimerAFreq_t) == 1, "TimerAFreq_t must be 1 byte");
_Static_assert(sizeof(TimerAValue_t) == 1, "TimerAValue_t must be 1 byte");
_Static_assert(sizeof(TimerBFreq_t) == 1, "TimerBFreq_t must be 1 byte");
_Static_assert(sizeof(TimerBValue_t) == 1, "TimerBValue_t must be 1 byte");

typedef union
{
    uint8_t       raw;           // Single uint8_t for raw data
    Control1_t    control1;      // Control Register 1 struct
    Control2_t    control2;      // Control Register 2 struct
    Control3_t    control3;      // Control Register 3 struct
    Seconds_t     seconds;       // Seconds Register struct
    Minutes_t     minutes;       // Minutes Register struct
    Hours_t       hours;         // Hours Register struct
    Days_t        days;          // Days Register struct
    Weekdays_t    weekdays;      // Weekdays Register struct
    Months_t      months;        // Months Register struct
    Years_t       years;         // Years Register struct
    MinuteAlarm_t minute_alarm;  // Minute Alarm Register struct
    HourAlarm_t   hour_alarm;    // Hour Alarm Register struct
    Offset_t      offset;        // Offset Register struct
    ClockOut_t    clock_out;     // CLKOUT Control Register struct
    TimerAFreq_t  timer_a_freq;  // Timer A Frequency Register struct
    TimerAValue_t timer_a_value; // Timer A Value Register struct
    TimerBFreq_t  timer_b_freq;  // Timer B Frequency Register struct
    TimerBValue_t timer_b_value; // Timer B Value Register struct
} Register_t;

static uint8_t integer_to_bcd(const uint8_t value)
{
    return (uint8_t)((value / 10) << 4) | (value % 10);
}

static uint8_t bcd_to_integer(const uint8_t value)
{
    return (uint8_t)((value >> 4) * 10 + (value & 0x0F));
}

ExitCode io_rtc_init(void)
{
    // 24-hour mode, no interrupts, oscillator running
    // select 7 pF capacitor instead of 12.5 pF
    Register_t control1;
    control1.raw = 0x00;

    ExitCode su = hw_i2c_memoryWrite(&rtc_i2c, REG_CONTROL_1, &control1.raw, sizeof(control1.raw));
    if (su != EXIT_CODE_OK)
    {
        LOG_ERROR("Failed to write to RTC control register 1");
        return false;
    }

    Register_t control3 = { 0 };
    su                  = hw_i2c_memoryWrite(&rtc_i2c, REG_CONTROL_3, &control3.raw, sizeof(control3.raw));
    if (su != EXIT_CODE_OK)
    {
        LOG_ERROR("Failed to write to RTC control register 3");
        return false;
    }

    // turn on the power switch to the RTC chip
    IoRtcTime time = { 0 };
    io_rtc_readTime(&time);
    LOG_INFO(
        "Current RTC TIME: %02d:%02d:%02d %02d/%02d/%02d", time.hours, time.minutes, time.seconds, time.day, time.month,
        time.year);

    return true;
}

ExitCode io_rtc_setTime(IoRtcTime *time)
{
    uint8_t seconds  = integer_to_bcd(time->seconds);
    uint8_t minutes  = integer_to_bcd(time->minutes);
    uint8_t hours    = integer_to_bcd(time->hours);
    uint8_t date     = integer_to_bcd(time->day);
    uint8_t weekdays = time->weekdays;
    uint8_t months   = integer_to_bcd(time->month);
    uint8_t years    = integer_to_bcd(time->year);

    Register_t regSecond      = { 0 };
    regSecond.seconds.SECONDS = (uint8_t)(seconds) & 0x7F;

    Register_t regMinutes      = { 0 };
    regMinutes.minutes.MINUTES = (uint8_t)(minutes) & 0x7F;

    Register_t regHours  = { 0 };
    regHours.hours.HOURS = (uint8_t)(hours) & 0x3F;

    Register_t regDays = { 0 };
    regDays.days.DAYS  = (uint8_t)(date);

    Register_t regWeekdays        = { 0 };
    regWeekdays.weekdays.WEEKDAYS = (uint8_t)(weekdays);

    Register_t regMonths    = { 0 };
    regMonths.months.MONTHS = (uint8_t)(months);

    Register_t regYears  = { 0 };
    regYears.years.YEARS = (uint8_t)(years);

    uint8_t buffer[7] = {
        regSecond.raw, regMinutes.raw, regHours.raw, regDays.raw, regWeekdays.raw, regMonths.raw, regYears.raw,
    };

    LOG_INFO(
        "Setting RTC time: %02d:%02d:%02d %02d/%02d/%02d", time->hours, time->minutes, time->seconds, time->year,
        time->month, time->day);
    LOG_INFO(
        "Writing to RTC: %02X %02X %02X %02X %02X %02X %02X", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4],
        buffer[5], buffer[6]);

    ExitCode su = hw_i2c_memoryWrite(&rtc_i2c, REG_SECONDS, buffer, sizeof(buffer));
    if (su != EXIT_CODE_OK)
    {
        LOG_ERROR("Failed to write to RTC time registers");
        return su;
    }
    return su;
}

ExitCode io_rtc_readTime(IoRtcTime *time)
{
    uint8_t  buffer[7];
    ExitCode su = hw_i2c_memoryRead(&rtc_i2c, REG_SECONDS, buffer, sizeof(buffer));
    if (su != EXIT_CODE_OK)
    {
        LOG_ERROR("Failed to read from RTC time registers");
        return su;
    }

    Register_t regSecond;
    regSecond.raw = buffer[0];

    Register_t regMinutes;
    regMinutes.raw = buffer[1];

    Register_t regHours;
    regHours.raw = buffer[2];

    Register_t regDays;
    regDays.raw = buffer[3];

    Register_t regWeekdays;
    regWeekdays.raw = buffer[4];

    Register_t regMonths;
    regMonths.raw = buffer[5];

    Register_t regYears;
    regYears.raw = buffer[6];

    time->seconds  = (uint8_t)(bcd_to_integer(regSecond.seconds.SECONDS));
    time->minutes  = (uint8_t)(bcd_to_integer(regMinutes.minutes.MINUTES));
    time->hours    = (uint8_t)(bcd_to_integer(regHours.hours.HOURS));
    time->day      = (uint8_t)(bcd_to_integer(regDays.days.DAYS));
    time->weekdays = (uint8_t)(regWeekdays.weekdays.WEEKDAYS);
    time->month    = (uint8_t)(bcd_to_integer(regMonths.months.MONTHS));
    time->year     = (uint8_t)(bcd_to_integer(regYears.years.YEARS));

    LOG_INFO(
        "Reading RTC time: %02d:%02d:%02d %02d/%02d/%02d", time->hours, time->minutes, time->seconds, time->year,
        time->month, time->day);
    LOG_INFO(
        "Read from RTC: %02X %02X %02X %02X %02X %02X %02X", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4],
        buffer[5], buffer[6]);
    return su;
}

void io_rtc_reset(void)
{
    Register_t control1;
    control1.raw = 0x58; // For a software reset, 01011000 (58h) must be sent to register Control_1 (see Section 8.3).
                         // Bit SR always returns 0 when read.

    ExitCode su = hw_i2c_memoryWrite(&rtc_i2c, REG_CONTROL_1, &control1.raw, sizeof(control1));
    if (su != EXIT_CODE_OK)
    {
        LOG_ERROR("Failed to write to RTC control register 1");
    }
}
