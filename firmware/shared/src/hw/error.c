#include "hw/error.h"
#include "io/log.h"
#include "hw/utils.h"
#include "main.h"

#if !(defined(LED_GPIO_Port) && defined(LED_Pin))
#warning "LED_GPIO_Port and LED_Pin are not defined. Please define them in main.h"
#endif

static char buffer[100];

void hw_error(const char *file, int line, const char *func)
{
    // Store the message into a buffer so we can easily inspect it using a
    // debugger even without SEGGER RTT set up
    snprintf(buffer, sizeof(buffer), "%s:%d %s: Called Error_Handler().", __BASENAME__(file), line, func);
    LOG_ERROR("%s", buffer);

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
