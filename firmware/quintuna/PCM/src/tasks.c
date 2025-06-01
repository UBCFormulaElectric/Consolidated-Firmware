#include "tasks.h"
#include "jobs.h"

#include "io_log.h"
#include "hw_hardFaultHandler.h"

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    SEGGER_RTT_Init();
    LOG_INFO("RSM reset!");
    jobs_init();
}
