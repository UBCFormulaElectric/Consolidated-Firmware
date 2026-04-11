#include "bootloader.hpp"
#include "bootloader.h"
#include "main.h"
#include <hw_can.hpp>
#include "hw_rtosTaskHandler.hpp"
#include "bootloader_fsm.hpp"

extern "C"
{
#include <app_commitInfo.h>
}

void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}

void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}

void tx_overflow_clear_callback(){};
void rx_overflow_clear_callback(){};

io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue", tx_overflow_callback, tx_overflow_clear_callback };
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue", rx_overflow_callback, rx_overflow_clear_callback };

namespace hw::cans
{
// no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
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
            git_commit_has_val,
            git_commit_clean_val){};
} fsm_boot_config;

void bootloader_preInit(void)
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024>
    TaskRunInterface(osPriorityRealtime, "TaskRunInterface", [](void *) { bootloader::runInterfaceTask(fsm_boot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunTickTask(osPriorityRealtime, "TaskRunTickTask", [](void *) { bootloader::runTickTask(fsm_boot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunCanTx(osPriorityRealtime, "TaskRunCanTx", [](void *) { bootloader::runCanTxTask(fsm_boot_config); });

[[noreturn]] void bootloader_init(void)
{
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, GPIO_PIN_SET);
    bootloader::init(fsm_boot_config);
    osKernelInitialize();
    TaskRunInterface.start();
    TaskRunTickTask.start();
    TaskRunCanTx.start();
    osKernelStart();
    forever {}
}