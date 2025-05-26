#include "hw_bootup.h"
#include "hw_utils.h"
#include "io_log.h"
#include <assert.h>
#include <cmsis_os.h>

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    LOG_ERROR("Stack overflow detected in task %s, resetting...", pcTaskName);

    const BootRequest request = { .target        = BOOT_TARGET_APP,
                                  .context       = BOOT_CONTEXT_STACK_OVERFLOW,
                                  .context_value = uxTaskGetTaskNumber(xTask) };
    hw_bootup_setBootRequest(request);
    BREAK_IF_DEBUGGER_CONNECTED();
    NVIC_SystemReset();
}
