#include "hw_bootup.h"
#include "hw_utils.h"
#include "io_log.h"
#include <assert.h>
#include <cmsis_os.h>
#include <projdefs.h>

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    LOG_ERROR("Stack overflow detected in task %s, resetting...", pcTaskName);

    TaskStatus_t status;
    vTaskGetInfo(xTask, &status, pdFALSE, eRunning);
    const BootRequest request = { .target        = BOOT_TARGET_APP,
                                  .context       = BOOT_CONTEXT_STACK_OVERFLOW,
                                  .context_value = status.xTaskNumber };
    hw_bootup_setBootRequest(request);

    BREAK_IF_DEBUGGER_CONNECTED();
    NVIC_SystemReset();
}
