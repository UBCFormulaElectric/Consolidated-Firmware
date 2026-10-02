#include "hw_error.h"
#include "io_log.h"
#include "hw_utils.h"
#include "main.h"

#if !(defined(LED_GPIO_Port) && defined(LED_Pin))
#warning "LED_GPIO_Port and LED_Pin are not defined. Please define them in main.h"
#endif

void hw_error(const char *file, int line, const char *func)
{
    // Log directly through RTT without heavy snprintf
    LOG_ERROR("Called Error_Handler() at %s:%d in %s", __BASENAME__(file), line, func);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap in a infinite loop.
#if (defined(LED_GPIO_Port) && defined(LED_Pin))
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        HAL_Delay(250);
#endif
    }
}
