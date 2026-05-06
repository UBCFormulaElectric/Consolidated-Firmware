#include "bootloader.hpp"
#include "bootloader_vc.h"
#include "bootloader_vc.hpp"
#include "hw_can.hpp"
#include "main.h"
#include "hw_rtosTaskHandler.hpp"
#include <cassert>

io::queue<hw::CanMsg, 256> boot_can_tx_queue("CanTxQueue");
io::queue<hw::CanMsg, 256> boot_can_rx_queue("CanRxQueue");

namespace hw::cans
{
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { LOG_IF_ERR(boot_can_rx_queue.push(msg)); }); // Lambda fucntion freaky
}

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    (void)hfdcan;
    return cans::fdcan1;
}

class VCBootConfig : public bootloader::config
{
  public:
    VCBootConfig()
      : bootloader::config(
            hw::cans::fdcan1,
            boot_can_tx_queue,
            boot_can_rx_queue,
            board_highbits,
            git_commit_has_val,
            git_commit_clean_val)
    {
    }
} vcboot_config;

CFUNC void bootloader_preinit()
{
    bootloader::preInit();
}

static hw::rtos::StaticTask::StaticTaskStack<1024> bootInterfaceStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> bootTickStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> bootCanTxStack;

static hw::rtos::StaticTask bootInterfaceTask(
    osPriorityRealtime,
    "BootIntf",
    [](void *) { bootloader::runInterfaceTask(vcboot_config); },
    bootInterfaceStack);
static hw::rtos::StaticTask bootTickTask(
    osPriorityRealtime,
    "BootTick",
    [](void *) { bootloader::runTickTask(vcboot_config); },
    bootTickStack);
static hw::rtos::StaticTask bootCanTxTask(
    osPriorityRealtime,
    "BootCanTx",
    [](void *) { bootloader::runCanTxTask(vcboot_config); },
    bootCanTxStack);

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(BOOT_GPIO_Port, BOOT_Pin, GPIO_PIN_SET);
    osKernelInitialize();
    bootloader::init(vcboot_config);
    UNUSED(bootInterfaceTask.start());
    UNUSED(bootTickTask.start());
    UNUSED(bootCanTxTask.start());
    osKernelStart();
    forever {}
}
