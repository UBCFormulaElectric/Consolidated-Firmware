#include "tasks.h"
#include "jobs.hpp"
#include "io_time.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_jsoncan.h"
#include "app_canAlerts.h"
#include "io_semaphore.h"
#include "io_canRx.h"
#include "io_canTx.h"
#include "io_canQueue.h"
}

#include "io_log.hpp"
#include "io_canMsg.hpp"

// hw
extern "C"
{
#include "hw_usb.h"
#include "hw_resetReason.h"
#include <FreeRTOS.h>
}

#include "hw_hardFaultHandler.hpp"
#include "hw_watchdog.hpp"
#include "hw_bootup.hpp"


void tasks_preInit() 
{
    hw::hardFaultHandler::init();
    hw::bootup::enableInterruptsForApp();
}
void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("BMS Reset");

    __HAL_DBGMCU_FREEZE_IWDG1();

    LOG_IF_ERR(hw_usb_init());
    jobs_init();

}

void tasks_run1Hz()
{
    const uint32_t period_ms = 1000U;
    const uint32_t watchdog_grace_period_ms = 50U;
    hw::watchdog::WatchdogInstance(TASK_INDEX_1HZ, period_ms + watchdog_grace_period_ms);

    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        hw::watchdog.checkIn();
        io::time::delayUntil(start_time + period_ms);
    }
}
void tasks_run100Hz()
{
    const uint32_t period_ms = 10U;
    const uint32_t watchdog_grace_period_ms = 2U;
    hw::watchdog::WatchdogInstance(TASK_INDEX_100HZ, period_ms + watchdog_grace_period_ms);
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run100Hz_tick();
        hw::watchdog.checkIn();
        io::time::delayUntil(start_time + period_ms);
    }
}
void tasks_run1kHz()
{
    const uint32_t period_ms = 1U;
    const uint32_t watchdog_grace_period_ms = 1U;
    hw::watchdog::WatchdogInstance(TASK_INDEX_1KHZ, period_ms + watchdog_grace_period_ms);
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1kHz_tick();
        hw::watchdog.checkIn();
        io::time::delayUntil(start_time + period_ms);
    }
}
void tasks_runCanTx()
{
    forever 
    {
#ifdef CHARGER_CAN
        // Elcon only supports regular CAN but we have some debug messages that are >8 bytes long. Use FDCAN for those
        // (they won't get seen by the charger, but they'll show up on CANoe).
        // TODO: Bit-rate-switching wasn't working for me when the BMS was connected to the charger, so the FD
        // peripheral is configured without BRS. Figure out why it wasn't working?

#else

#endif
    }
}
void tasks_runCanRx()
{
    forever {}
}
void tasks_runChimera()
{
    forever {}
}