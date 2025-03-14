
#include <stdint.h>

// Control Register 1
typedef struct __attribute__((packed))
{
    uint8_t CIE : 1;
    uint8_t AIE : 1;
    uint8_t SIE : 1;
    uint8_t _12_24 : 1;
    uint8_t SR : 1;
    uint8_t STOP : 1;
    uint8_t T : 1; // Must always be written with logic 0
    uint8_t CAP_SEL : 1;
} Control1_t;

// Control Register 2
typedef struct __attribute__((packed))
{
    uint8_t CTBIE : 1;
    uint8_t CTAIE : 1;
    uint8_t WTAIE : 1;
    uint8_t AF : 1;
    uint8_t SF : 1;
    uint8_t CTBF : 1;
    uint8_t CTAF : 1;
    uint8_t WTAF : 1;
} Control2_t;

// Control Register 3
typedef struct __attribute__((packed))
{
    uint8_t BLIE : 1;
    uint8_t BSIE : 1;
    uint8_t BLF : 1;
    uint8_t BSF : 1;
    uint8_t PM : 3; // PM[2:0]
    uint8_t RESERVED : 1;
} Control3_t;

// Time and Date Registers
typedef struct __attribute__((packed))
{
    uint8_t SECONDS : 6; // 0 to 59
    uint8_t OS : 1;
    uint8_t RESERVED : 1;
} Seconds_t;

typedef struct __attribute__((packed))
{
    uint8_t MINUTES : 7; // 0 to 59
    uint8_t RESERVED : 1;
} Minutes_t;

typedef struct __attribute__((packed))
{
    uint8_t HOURS : 5; // 0-23 or 1-12 in 12-hour mode
    uint8_t AMPM : 1;  // 0 = AM, 1 = PM (12-hour mode only)
    uint8_t RESERVED : 2;
} Hours_t;

typedef struct __attribute__((packed))
{
    uint8_t DAYS : 6; // 1 to 31
    uint8_t RESERVED : 2;
} Days_t;

typedef struct __attribute__((packed))
{
    uint8_t WEEKDAYS : 3; // 0 to 6
    uint8_t RESERVED : 5;
} Weekdays_t;

typedef struct __attribute__((packed))
{
    uint8_t MONTHS : 4; // 1 to 12
    uint8_t RESERVED : 4;
} Months_t;

typedef struct __attribute__((packed))
{
    uint8_t YEARS : 7; // 0 to 99
    uint8_t RESERVED : 1;
} Years_t;

// Alarm Registers
typedef struct __attribute__((packed))
{
    uint8_t MINUTE_ALARM : 6; // 0 to 59
    uint8_t AEN_M : 1;
    uint8_t RESERVED : 1;
} MinuteAlarm_t;

typedef struct __attribute__((packed))
{
    uint8_t HOUR_ALARM : 5; // 0-23 or 1-12 in 12-hour mode
    uint8_t AMPM : 1;
    uint8_t AEN_H : 1;
    uint8_t RESERVED : 1;
} HourAlarm_t;

typedef struct __attribute__((packed))
{
    uint8_t DAY_ALARM : 6; // 1 to 31
    uint8_t AEN_D : 1;
    uint8_t RESERVED : 1;
} DayAlarm_t;

typedef struct __attribute__((packed))
{
    uint8_t WEEKDAY_ALARM : 3; // 0 to 6
    uint8_t AEN_W : 1;
    uint8_t RESERVED : 4;
} WeekdayAlarm_t;

// Offset Register
typedef struct __attribute__((packed))
{
    uint8_t OFFSET : 7;
    uint8_t MODE : 1;
} Offset_t;

typedef struct __attribute__((packed))
{
    uint8_t OFFSET : 7;
    uint8_t MODE : 1;
} Offset_t;

typedef struct __attribute__((packed))
{
    uint8_t OFFSET : 7;
    uint8_t MODE : 1;
} Offset_t;

typedef struct __attribute__((packed))
{
    uint8_t OFFSET : 7;
    uint8_t MODE : 1;
} Offset_t;

