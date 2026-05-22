#include "bootloader.hpp"
#include "bootloader.h"
#include "main.h"
#include "hw_can.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "bootloader_h7.hpp"
#include "app_commitInfo.hpp"

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
io::queue<hw::CanMsg, 256> boot_can_tx_queue{ "CanTxQueue" };
static_assert(sizeof(boot_can_tx_queue) == 18544);
io::queue<hw::CanMsg, 256> boot_can_rx_queue{ "CanRxQueue" };
static_assert(sizeof(boot_can_rx_queue) == 18544);

namespace hw::cans
{
// no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
fdcan fdcan1(hfdcan2, [](const hw::CanMsg &msg) { UNUSED(boot_can_rx_queue.push(msg)); });
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
            GIT_COMMIT_HASH,
            GIT_COMMIT_CLEAN){};
} h7devboot_config;

void bootloader_preInit()
{
    bootloader::preInit();
}

static hw::rtos::StaticTask::StaticTaskStack<1024> TaskRunInterfaceStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> TaskRunTickTaskStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> TaskRunCanTxStack;

static hw::rtos::StaticTask TaskRunInterface(
    osPriorityRealtime,
    "bootinf",
    [](void *) { bootloader::runInterfaceTask(h7devboot_config); },
    TaskRunInterfaceStack);
static hw::rtos::StaticTask TaskRunTickTask(
    osPriorityRealtime,
    "boottick",
    [](void *) { bootloader::runTickTask(h7devboot_config); },
    TaskRunTickTaskStack);
static hw::rtos::StaticTask TaskRunCanTx(
    osPriorityRealtime,
    "boottx",
    [](void *) { bootloader::runCanTxTask(h7devboot_config); },
    TaskRunCanTxStack);

[[noreturn]] void bootloader_init()
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