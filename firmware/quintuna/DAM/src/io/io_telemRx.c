#include "io_telemRx.h"
#include "hw_uart.h"
#include "hw_uarts.h"

#define BUFFER_SIZE sizeof(IoRtcTime) + 2

static IoRtcTime received_time_data = { 0 };

static uint8_t data[BUFFER_SIZE];
void           io_telemRx()
{
    // for now we just simply recieve the data and set the time
    ExitCode c = hw_uart_receive_pooling(&_900k_uart, data, BUFFER_SIZE);
    if (c != EXIT_CODE_OK)
        return;
    if (data[0] == 0xFF && data[1] == 0x01)
    {
        // Extract the time data from the received data
        received_time_data.seconds  = data[2];
        received_time_data.minutes  = data[3];
        received_time_data.hours    = data[4];
        received_time_data.day      = data[5];
        received_time_data.weekdays = data[6];
        received_time_data.month    = data[7];
        received_time_data.year     = data[8];

        io_rtc_setTime(&received_time_data);
        IoRtcTime time;
        io_rtc_readTime(&time);
    }
}