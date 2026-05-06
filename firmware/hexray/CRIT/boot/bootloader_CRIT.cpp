#include "bootloader.hpp"
#include "bootloader_CRIT.hpp"
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

static_assert(sizeof(hw::CanMsg) == 72);
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

class HexrayCRITBootConfig final : public bootloader::config
{
  public:
    HexrayCRITBootConfig()
      : bootloader::config(
            hw::cans::fdcan1,
            boot_can_tx_queue,
            boot_can_rx_queue,
            board_highbits,
            git_commit_hash_val,
            git_commit_clean_val)
    {
    }
} hexray_CRIT_boot_config;

CFUNC void bootloader_preinit()
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024> bootInterfaceTask(
    osPriorityRealtime,
    "BootIntf",
    [](void *) { bootloader::runInterfaceTask(hexray_CRIT_boot_config); });
static hw::rtos::StaticTask<1024>
    bootTickTask(osPriorityRealtime, "BootTick", [](void *) { bootloader::runTickTask(hexray_CRIT_boot_config); });
static hw::rtos::StaticTask<1024>
    bootCanTxTask(osPriorityRealtime, "BootCanTx", [](void *) { bootloader::runCanTxTask(hexray_CRIT_boot_config); });

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(LED_BOOT_GPIO_Port, LED_BOOT_Pin, GPIO_PIN_SET);
    osKernelInitialize();
    bootloader::init(hexray_CRIT_boot_config);
    bootInterfaceTask.start();
    bootTickTask.start();
    bootCanTxTask.start();
    osKernelStart();
    forever {}
}
