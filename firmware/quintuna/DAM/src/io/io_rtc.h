
struct IoRtcTime
{
    uint8_t seconds : 6;
    uint8_t minutes : 7;
    uint8_t hours : 5;
    uint8_t day : 6;
    uint8_t date : 6;
    uint8_t month;
    uint8_t year;
};

struct IoRtcHealth
{
    bool batteryLow : 1;
    // maybe add more health checks here
};

// always 24-hour mode

// any initialization setup for the RTC chip
void io_rtc_init(void);

// set the time on the RTC chip
void io_rtc_setTime(struct IoRtcTime *time);

// read the time from the RTC chip
void io_rtc_readTime(struct IoRtcTime *time);

// check the health of the RTC chip
struct IoRtcHealth io_rtc_healthCheck(void);

// reset the RTC chip
void io_rtc_reset(void);
