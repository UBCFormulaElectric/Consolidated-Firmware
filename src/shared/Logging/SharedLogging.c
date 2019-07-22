/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedLogging.h"

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedLogging_Printf(const char *sFormat, ...)
{
    va_list ParamList;

    va_start(ParamList, sFormat);
    (void)SEGGER_RTT_vprintf(0, sFormat, &ParamList);
    va_end(ParamList);
}
