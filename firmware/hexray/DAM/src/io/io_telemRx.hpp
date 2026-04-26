#include "io_rtc.hpp"
#include <stdbool.h>
#include <stdint.h>

bool isLeapYear(uint16_t year);
uint8_t daysInMonth(uint16_t year, uint8_t month);
uint64_t RtcTimeToSeconds(io::rtc::Time t);
uint8_t calcWeekday(uint16_t year, uint8_t month, uint8_t day);
io::rtc::Time SecondsToRtcTime(uint64_t totalSeconds);

void io_telemRx(void);
void transmitNTPStartMsg(void);
void pollForRadioMessages(void);
void parseNTPPacketBody(uint8_t rxBufferBody[]);
void tuneRTC(void);

typedef struct
{
    io::rtc::Time t0;
    io::rtc::Time t1;
    io::rtc::Time t2;
    io::rtc::Time t3;
} NTPTimestamps;