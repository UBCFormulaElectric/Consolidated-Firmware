#include "bootloader.hpp"
#include "bootloader_DAM.h"
#include "bootloader_DAM.hpp"
#include "hw_can.hpp"
#include "main.h"
#include "hw_rtosTaskHandler.hpp"
#include <cassert>

io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue" };
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue" };

namespace hw::cans
{
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
}

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    (void)hfdcan;
    return cans::fdcan1;
}

class DAMBootConfig : public bootloader::config
{
  public:
    DAMBootConfig()
      : bootloader::config(
            hw::cans::fdcan1,
            boot_can_tx_queue,
            boot_can_rx_queue,
            board_highbits,
            git_commit_has_val,
            git_commit_clean_val)
    {
    }
} dam_boot_config;

CFUNC void bootloader_preinit()
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024>
    bootInterfaceTask(osPriorityRealtime, "BootIntf", [](void *) { bootloader::runInterfaceTask(dam_boot_config); });
static hw::rtos::StaticTask<1024>
    bootTickTask(osPriorityRealtime, "BootTick", [](void *) { bootloader::runTickTask(dam_boot_config); });
static hw::rtos::StaticTask<1024>
    bootCanTxTask(osPriorityRealtime, "BootCanTx", [](void *) { bootloader::runCanTxTask(dam_boot_config); });

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(BOOT_LED_GPIO_Port, BOOT_LED_Pin, GPIO_PIN_SET);
    osKernelInitialize();
    bootloader::init(dam_boot_config);
    bootInterfaceTask.start();
    bootTickTask.start();
    bootCanTxTask.start();
    osKernelStart();
    forever {};
}