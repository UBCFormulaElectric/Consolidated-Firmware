#include "Io_SharedLogging.h"
#include "SEGGER_RTT_Conf.h"

// RTT channel 0 is always present and reserved for Terminal usage. Name is
// fixed to "Terminal".
#define SEGGER_RTT_TERMINAL_CHANNEL (0)

void Io_SharedLogging_Printf(const char *sFormat, ...)
{
    va_list ParamList;

    va_start(ParamList, sFormat);
    (void)SEGGER_RTT_vprintf(SEGGER_RTT_TERMINAL_CHANNEL, sFormat, &ParamList);
    va_end(ParamList);
}
