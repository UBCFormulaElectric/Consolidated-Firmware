#include "bootloader.hpp"
#include "bootloader_RSM.hpp"
#include "bootloader.h"
#include "main.h"
#include "hw_rtosTaskHandler.hpp"
#include <cassert>

static_assert(sizeof(hw::CanMsg) == 72);
io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue" };
static_assert(sizeof(boot_can_tx_queue) == 18544);
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue" };
static_assert(sizeof(boot_can_rx_queue) == 18544);

namespace hw::cans
{
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); });
}

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return hw::cans::fdcan1;
}

bootloader::config RSM_boot_config{ hw::cans::fdcan1, boot_can_tx_queue,   boot_can_rx_queue,
                                    board_highbits,   git_commit_hash_val, git_commit_clean_val };

static hw::rtos::StaticTask::StaticTaskStack<1024> bootInterfaceStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> bootTickStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> bootCanTxStack;

static hw::rtos::StaticTask bootInterfaceTask(
    osPriorityRealtime,
    "BootIntf",
    [](void *) { bootloader::runInterfaceTask(RSM_boot_config); },
    bootInterfaceStack);
static hw::rtos::StaticTask bootTickTask(
    osPriorityRealtime,
    "BootTick",
    [](void *) { bootloader::runTickTask(RSM_boot_config); },
    bootTickStack);
static hw::rtos::StaticTask bootCanTxTask(
    osPriorityRealtime,
    "BootCanTxTask",
    [](void *) { bootloader::runCanTxTask(RSM_boot_config); },
    bootCanTxStack);

CFUNC void bootloader_preInit()
{
    bootloader::preInit();
}

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(BOOT_GPIO_Port, BOOT_Pin, GPIO_PIN_SET);
    osKernelInitialize();
    bootloader::init(RSM_boot_config);
    UNUSED(bootInterfaceTask.start());
    UNUSED(bootTickTask.start());
    UNUSED(bootCanTxTask.start());
    osKernelStart();
    forever{};
}
