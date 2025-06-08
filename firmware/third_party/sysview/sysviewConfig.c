#include "FreeRTOS.h"
#include "SEGGER_SYSVIEW.h"

#include <SEGGER_RTT.h>
#include <string.h>

// EXTERNS
extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;
void                               hw_sysviewConfig_sendSystemDesc(void); // define this in a hw_sysviewDescConfig.c

// Frequency of the timestamp. Must match SEGGER_SYSVIEW_GET_TIMESTAMP in SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ (configCPU_CLOCK_HZ)
// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ configCPU_CLOCK_HZ
// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE (0x10000000)
void SEGGER_SYSVIEW_Conf(void)
{
    memset(&_SEGGER_RTT, 0, sizeof(_SEGGER_RTT)); // Clear RTT buffer
    SEGGER_SYSVIEW_Init(
        SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ, &SYSVIEW_X_OS_TraceAPI, hw_sysviewConfig_sendSystemDesc);
    SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}

// U32 SEGGER_SYSVIEW_X_GetInterruptId(void)
// {
//     return (ICSR & 0x1FF) >> 4;
// };
