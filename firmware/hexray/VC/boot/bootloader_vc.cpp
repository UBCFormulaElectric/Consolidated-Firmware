#include "bootloader.hpp"
#include "bootloader_vc.h"
#include "bootloader_vc.hpp"
#include "hw_can.hpp"
#include "main.h"
#include "hw_rtosTaskHandler.hpp"
#include <cassert>

static void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}
static void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
}
static void rx_overflow_clear_callback(){};
static void tx_overflow_clear_callback(){};

io::queue<hw::CanMsg, 256> boot_can_tx_queue("CanTxQueue", tx_overflow_callback, tx_overflow_clear_callback);
io::queue<hw::CanMsg, 256> boot_can_rx_queue("CanRxQueue", rx_overflow_callback, rx_overflow_clear_callback);

namespace hw::cans
{
fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { (void)boot_can_rx_queue.push(msg); }); // Lambda fucntion freaky
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
    VCBootConfig(): bootloader::config(
        hw::cans::fdcan1, boot_can_tx_queue,
        boot_can_rx_queue, board_highbits,
        git_commit_has_val, git_commit_clean_val){}
} vcboot_config;

CFUNC void bootloader_preinit()
{
    bootloader::preInit();
}

static hw::rtos::StaticTask<1024> bootInterfaceTask(
    osPriorityRealtime,
    "BootIntf",
    [](void *) { bootloader::runInterfaceTask(vcboot_config); });
static hw::rtos::StaticTask<1024>
    bootTickTask(osPriorityRealtime, "BootTick", [](void *) { bootloader::runTickTask(vcboot_config); });
static hw::rtos::StaticTask<1024>
    bootCanTxTask(osPriorityRealtime, "BootCanTx", [](void *) { bootloader::runCanTxTask(vcboot_config); });

[[noreturn]] void bootloader_init()
{
    HAL_GPIO_WritePin(BOOT_GPIO_Port, BOOT_Pin, GPIO_PIN_SET);
    osKernelInitialize();
    bootloader::init(vcboot_config);
    bootInterfaceTask.start();
    bootTickTask.start();
    bootCanTxTask.start();
    osKernelStart();
    forever {}
}
