#include "bootloader.hpp"
#include "bootloader.h"
#include "main.h"
#include "hw_can.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "bootloader_fsm.hpp"
extern "C"
{
#include "app_commitInfo.h"
}

#include <cassert>

static_assert(sizeof(hw::CanMsg) == 72);
io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue" };
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue" };

namespace hw::cans
{
// no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
fdcan fdcan1(hfdcan1, [](const CanMsg &msg) { LOG_IF_ERR(boot_can_rx_queue.push(msg)); });
} // namespace hw::cans

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return cans::fdcan1;
}

class FSMBootConfig : public bootloader::config
{
  public:
    FSMBootConfig()
      : bootloader::config(
            hw::cans::fdcan1,
            boot_can_tx_queue,
            boot_can_rx_queue,
            board_highbits,
            GIT_COMMIT_HASH,
            GIT_COMMIT_CLEAN){};
} fsm_boot_config;

CFUNC void bootloader_preInit(void)
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024> TaskRunInterface(
    osPriorityRealtime,
    "TaskRunInterface",
    [](void *) { bootloader::runInterfaceTask(fsm_boot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunTickTask(osPriorityRealtime, "TaskRunTickTask", [](void *) { bootloader::runTickTask(fsm_boot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunCanTx(osPriorityRealtime, "TaskRunCanTx", [](void *) { bootloader::runCanTxTask(fsm_boot_config); });

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(BOOT_LED_GPIO_Port, BOOT_LED_Pin, GPIO_PIN_SET);
    bootloader::init(fsm_boot_config);
    osKernelInitialize();
    TaskRunInterface.start();
    TaskRunTickTask.start();
    TaskRunCanTx.start();
    osKernelStart();
    forever {}
}