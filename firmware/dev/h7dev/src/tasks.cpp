#include "tasks.h"

#include "SEGGER_SYSVIEW.h"
#include "cmsis_os.h"
#include "hw/hw_cans.hpp"
#include "io_canQueues.hpp"
#include "io_log.hpp"
#include "hw_cans.hpp"
#include "io_canQueues.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "main.h"
#include <assert.h>
#include <cmsis_os2.h>
#include "io_canMsg.hpp"

[[noreturn]] static void tasks_default(void *args)
{
    forever
    {
        osDelay(osWaitForever);
    }
}

[[noreturn]] static void tasks_canRx(void *args)
{
    forever
    {
        const io::CanMsg msg = can_rx_queue.pop().value();
        UNUSED(msg);
    }
}

[[noreturn]] static void tasks_canTx(void *args)
{
    forever
    {
        const io::CanMsg msg = can_tx_queue.pop().value();
        hw::can::fdcan1.can_transmit(hw::CanMsg{
            msg.std_id,
            msg.dlc,
            msg.data,
        });
    }
}

static hw::rtos::StaticTask<512> TaskCanRx(osPriorityBelowNormal, "TaskCanRx", tasks_canRx);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityBelowNormal, "TaskCanTx", tasks_canTx);
static hw::rtos::StaticTask<512> TaskDefault(osPriorityAboveNormal, "TaskDefault", tasks_default);

[[noreturn]] void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h7dev reset!");
    hw_hardFaultHandler_init();
    hw::can::fdcan1.init();
    hw::can::fdcan2.init();
    can_tx_queue.init();
    can_rx_queue.init();

    osKernelInitialize();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskDefault.start();

    osKernelStart();
    forever{};
    // __HAL_DBGMCU_FREEZE_IWDG();
}