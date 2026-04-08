#include "io_rtc.hpp"
#include <stdbool.h>
#include <stdint.h>
#include <span>

#pragma once

void          io_telemRx(void);
void          transmitNTPStartMsg(void);
void          pollForRadioMessages(void);
void          parseNTPPacketBody(std::span<uint8_t> body);
void          tuneRTC(void);
uint64_t      RtcTimeToMs(io::rtc::Time t);
io::rtc::Time MsToRtcTime(uint64_t ms);

void asyncUartTest();
void receiveTest();

typedef struct
{
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
} NTPTimestamps;
