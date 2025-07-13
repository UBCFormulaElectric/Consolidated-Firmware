#include "io_telemBaseTime.h"
#include "io_rtc.h"
#include "io_canMsg.h"
#include "io_telemMessage.h"

static CanMsg start_time_msg = {
    .std_id = 0x999,
    .dlc    = 6,
};

static IoRtcTime start_time;

ExitCode io_telemBaseTimeInit()
{
    const ExitCode status = io_rtc_readTime(&start_time);
    if (status != EXIT_CODE_OK)
    {
        return status;
    }

    start_time_msg.data.data8[0] = start_time.year;
    start_time_msg.data.data8[1] = start_time.month;
    start_time_msg.data.data8[2] = start_time.day;
    start_time_msg.data.data8[3] = start_time.hours;
    start_time_msg.data.data8[4] = start_time.minutes;
    start_time_msg.data.data8[5] = start_time.seconds;

    return EXIT_CODE_OK;
}

ExitCode io_telemBaseTimeSend()
{
    io_telemMessage_pushMsgtoQueue(&start_time_msg);
    return EXIT_CODE_OK;
}
