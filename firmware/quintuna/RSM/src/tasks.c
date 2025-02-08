#include "tasks.h"

#include "io_coolants.h"


#include "hw_pwms.h"
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"



void canRxQueueOverflowCallBack(uint32_t overflow_count) {

}

void canTxQueueOverflowCallBack(uint32_t overflow_count) {

}

void canTxQueueOverflowClearCallback(void) {

}

void canRxQueueOverflowClearCallback(void) {

}

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
}

static void jsoncan_transmit()
{

}

void tasks_init() {
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    hw_adcs_chipsInit();

    hw_hardFaultHandler_init();
    hw_hardware_init(hw_watch)

}



void HAL_TIM_IC_CaptureCallback()
{
    io_coolant_inputCaptureCallback(&coolant_flow_meter);
}
