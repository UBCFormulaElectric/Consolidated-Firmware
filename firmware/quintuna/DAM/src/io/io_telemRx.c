#include "io_telemRx.h"
#include "hw_uart.h"
#include "hw_uarts.h"

static IoRtcTime received_time_data = { 0 };

void io_telemRx()
{
    // for now we just simply recieve the data and set the time
    uint8_t data[sizeof(IoRtcTime)];
    hw_uart_receive(&_900k_uart, data, sizeof(IoRtcTime));
    if (data[0] == 0xAA && data[1] == 0x55)
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
    }
}