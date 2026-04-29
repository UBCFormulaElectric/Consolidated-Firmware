#include "bootloader.hpp"
#include "bootloader_RSM.hpp"
#include "bootloader.h"
#include "main.h"
#include "io_log.hpp"
#include "hw_rtosTaskHandler.hpp"
#include <cassert>

void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_WARN("CAN TX queue overflow");
}

void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_WARN("CAN RX queue overflow");
}

void tx_overflow_clear_callback(){};
void rx_overflow_clear_callback(){};


static_assert(sizeof(hw::CanMsg) == 72);
io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue", tx_overflow_callback, tx_overflow_clear_callback };
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue", rx_overflow_callback, rx_overflow_clear_callback };

namespace hw::cans
{
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
}

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return hw::cans::fdcan1;
}

bootloader::config RSM_boot_config(
    hw::cans::fdcan1,
    boot_can_tx_queue,
    boot_can_rx_queue,
    board_highbits,
    git_commit_hash_val,
    git_commit_clean_val);


static hw::rtos::StaticTask<1024>
    bootInterfaceTask(osPriorityRealtime, "BootIntf", [](void *) { bootloader::runInterfaceTask(RSM_boot_config); });
static hw::rtos::StaticTask<1024>
    bootTickTask(osPriorityRealtime, "BootTick", [](void *) { bootloader::runTickTask(RSM_boot_config); });
static hw::rtos::StaticTask<1024>
    bootCanTxTask(osPriorityRealtime, "BootCanTxTask", [](void *) { bootloader::runCanTxTask(RSM_boot_config); });

CFUNC void bootloader_preInit()
{
    bootloader::preInit();
}

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    osKernelInitialize();
    bootloader::init(RSM_boot_config);
    bootInterfaceTask.start();
    bootTickTask.start();
    bootCanTxTask.start();
    osKernelStart();
    forever {};
}
