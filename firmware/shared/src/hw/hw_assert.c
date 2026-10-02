#include "app_utils.h"
#include "hw_utils.h"
#include "io_log.h"
#include "main.h"

#if !(defined(LED_GPIO_Port) && defined(LED_Pin))
#warning "LED_GPIO_Port and LED_Pin are not defined. Please define them in main.h"
#endif

void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
    // Log directly through RTT without heavy snprintf
    LOG_ERROR("Assertion failed: %s at %s:%d in %s", failedexpr, __BASENAME__(file), line, func);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap in a infinite loop, waiting for the hardware watchdog to reset
#if (defined(LED_GPIO_Port) && defined(LED_Pin))
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        HAL_Delay(250);
#endif
    }
}
