#include "bootloader.hpp"
#include "main.h"
#include <hw_can.hpp>
#include "hw_rtosTaskHandler.hpp"

namespace hw::cans
{
// no tasks_runCanRxCallback yet in tasks.c (need bootloader stuff)
fdcan fdcan1(hfdcan1, 0, [](const io::CanMsg &msg) { UNUSED(msg); });
} // namespace hw::cans

// should I change type to FDCAN_HandleTypeDef in hw_can.hpp?
const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return cans::fdcan1;
}

class H5DevBootConfig : public bootloader::config
{
  public:
    H5DevBootConfig(hw::fdcan fdcan_handle_in) : fdcan_handle(fdcan_handle_in){};
} h5devboot_config(hw::cans::fdcan1);


void bootloader_preInit(void)
{
    bootloader::preInit();
    HAL_GPIO_WritePin(BOOT_GPIO_Port, BOOT_Pin, GPIO_PIN_SET);
}

static hw::rtos::StaticTask<1024>
    TaskRunInterface(osPriorityRealtime, "TaskChimera", [](void *) { bootloader::runInterfaceTask(h5devboot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunTickTask(osPriorityRealtime, "TaskChimera", [](void *) { bootloader::runTickTask(h5devboot_config); });
static hw::rtos::StaticTask<1024>
    TaskRunCanTx(osPriorityRealtime, "TaskChimera", [](void *) { bootloader::runCanTxTask(h5devboot_config); });

void bootloader_init(void)
{
    bootloader::init();
    osKernelInitialize();
    TaskRunInterface.start();
    TaskRunTickTask.start();
    TaskRunCanTx.start();
    osKernelStart();
    forever {}
}