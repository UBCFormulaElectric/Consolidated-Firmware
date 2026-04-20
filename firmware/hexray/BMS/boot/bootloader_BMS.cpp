#include "bootloader.hpp"
#include "bootloader_BMS.hpp"
#include "bootloader.h"
#include "hw_can.hpp"
#include "main.h"
#include "hw_rtosTaskHandler.hpp"
#include <cassert>

void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}
void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}
void rx_overflow_clear_callback(){};
void tx_overflow_clear_callback(){};

io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue", tx_overflow_callback, tx_overflow_clear_callback };
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue", rx_overflow_callback, rx_overflow_clear_callback };

namespace hw::cans
{
fdcan fdcan2(hfdcan2, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
}

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan2.getHfdcan());
    (void)hfdcan;
    return cans::fdcan2;
}

class HexrayBMSBootConfig final : public bootloader::config
{
  public:
    HexrayBMSBootConfig()
      : bootloader::config(
            hw::cans::fdcan2,
            boot_can_tx_queue,
            boot_can_rx_queue,
            board_highbits,
            git_commit_has_val,
            git_commit_clean_val)
    {
    }
} hexray_bms_boot_config;

CFUNC void bootloader_preinit()
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024> bootInterfaceTask(
    osPriorityRealtime,
    "BootIntf",
    [](void *) { bootloader::runInterfaceTask(hexray_bms_boot_config); });
static hw::rtos::StaticTask<1024>
    bootTickTask(osPriorityRealtime, "BootTick", [](void *) { bootloader::runTickTask(hexray_bms_boot_config); });
static hw::rtos::StaticTask<1024>
    bootCanTxTask(osPriorityRealtime, "BootCanTx", [](void *) { bootloader::runCanTxTask(hexray_bms_boot_config); });

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
    osKernelInitialize();
    bootloader::init(hexray_bms_boot_config);
    bootInterfaceTask.start();
    bootTickTask.start();
    bootCanTxTask.start();
    osKernelStart();
    forever {}
}
