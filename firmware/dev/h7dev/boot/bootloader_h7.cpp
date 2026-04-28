#include "bootloader.hpp"
#include "bootloader.h"
#include "main.h"
#include <hw_can.hpp>
#include "hw_rtosTaskHandler.hpp"
#include "bootloader_h7.hpp"

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

static_assert(sizeof(hw::CanMsg) == 72);
io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue", tx_overflow_callback, tx_overflow_clear_callback };
static_assert(sizeof(boot_can_tx_queue) == 18556);
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue", rx_overflow_callback, rx_overflow_clear_callback };
static_assert(sizeof(boot_can_rx_queue) == 18556);

namespace hw::cans
{
// no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
fdcan fdcan1(hfdcan2, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
} // namespace hw::cans

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return cans::fdcan1;
}

class H7DevBootConfig : public bootloader::config
{
  public:
    H7DevBootConfig()
      : bootloader::config(
            hw::cans::fdcan1,
            boot_can_tx_queue,
            boot_can_rx_queue,
            board_highbits,
            git_commit_has_val,
            git_commit_clean_val){};
} h7devboot_config;

void bootloader_preInit(void)
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024>
    TaskRunInterface(osPriorityRealtime, "bootinf", [](void *) { bootloader::runInterfaceTask(h7devboot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunTickTask(osPriorityRealtime, "boottick", [](void *) { bootloader::runTickTask(h7devboot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunCanTx(osPriorityRealtime, "boottx", [](void *) { bootloader::runCanTxTask(h7devboot_config); });

[[noreturn]] void bootloader_init(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    bootloader::init(h7devboot_config);
    osKernelInitialize();
    TaskRunInterface.start();
    TaskRunTickTask.start();
    TaskRunCanTx.start();
    osKernelStart();
    forever {}
}