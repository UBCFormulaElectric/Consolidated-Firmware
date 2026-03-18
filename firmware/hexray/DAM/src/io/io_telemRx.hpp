#include "io_rtc.hpp"
#include <stdbool.h>
#include <stdint.h>

bool isLeapYear(uint16_t year);
uint8_t daysInMonth(uint16_t year, uint8_t month);
uint64_t IoRtcTimeToSeconds(IoRtcTime t);
uint8_t calcWeekday(uint16_t year, uint8_t month, uint8_t day);
IoRtcTime SecondsToIoRtcTime(uint64_t totalSeconds);

void io_telemRx(void);
void transmitNTPStartMsg(void);
void pollForRadioMessages(void);
void parseNTPPacketBody(uint8_t rxBufferBody[]);
void tuneRTC(void);

typedef struct
{
    IoRtcTime t0;
    IoRtcTime t1;
    IoRtcTime t2;
    IoRtcTime t3;
} NTPTimestamps;