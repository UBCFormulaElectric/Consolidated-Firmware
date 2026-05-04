#include "bootloader.hpp"
#include "bootloader.h"
#include "main.h"
#include "hw_can.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "bootloader_h5.hpp"

static_assert(sizeof(hw::CanMsg) == 72);
io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue" };
static_assert(sizeof(boot_can_tx_queue) == 18544);
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue" };
static_assert(sizeof(boot_can_rx_queue) == 18544);

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

class H5DevBootConfig : public bootloader::config
{
  public:
    H5DevBootConfig()
      : bootloader::config(
            hw::cans::fdcan1,
            boot_can_tx_queue,
            boot_can_rx_queue,
            board_highbits,
            git_commit_hash_val,
            git_commit_clean_val){};
} h5devboot_config;

void bootloader_preInit(void)
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024>
    TaskRunInterface(osPriorityRealtime, "TaskChimera", [](void *) { bootloader::runInterfaceTask(h5devboot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunTickTask(osPriorityRealtime, "TaskChimera", [](void *) { bootloader::runTickTask(h5devboot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunCanTx(osPriorityRealtime, "TaskChimera", [](void *) { bootloader::runCanTxTask(h5devboot_config); });

[[noreturn]] void bootloader_init(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    bootloader::init(h5devboot_config);
    osKernelInitialize();
    TaskRunInterface.start();
    TaskRunTickTask.start();
    TaskRunCanTx.start();
    osKernelStart();
    forever {}
}