#include "hw_bootup.hpp"
#include "hw_utils.hpp"
#include "io_log.hpp"
#include <cassert>
#include <cmsis_os.h>
#include <projdefs.h>

_Noreturn void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    LOG_ERROR("Stack overflow detected in task %s, resetting...", pcTaskName);

#ifndef NO_BOOTLOADER
    TaskStatus_t status;
    vTaskGetInfo(xTask, &status, pdFALSE, eRunning);
    const hw::bootup::BootRequest request = { .target        = hw::bootup::BootTarget::BOOT_TARGET_APP,
                                              .context       = hw::bootup::BootContext::BOOT_CONTEXT_STACK_OVERFLOW,
                                              ._unused       = 0xFFFF,
                                              .context_value = status.xTaskNumber };
    hw::bootup::setBootRequest(request);
#endif

    BREAK_IF_DEBUGGER_CONNECTED();
    NVIC_SystemReset();
}
