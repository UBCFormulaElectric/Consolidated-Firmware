#include <stdio.h>
#include "utils.h"
#include "hw_utils.h"
#include "io_log.h"
#include "main.h"

#if !(defined(LED_GPIO_Port) && defined(LED_Pin))
#warning "LED_GPIO_Port and LED_Pin are not defined. Please define them in main.h"
#endif

static char buffer[100];

void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
    // Store the message into a buffer so we can easily inspect it using a
    // debugger even without SEGGER RTT set up
    snprintf(buffer, sizeof(buffer), "%s:%d %s: Assertion `%s' failed\r\n", __BASENAME__(file), line, func, failedexpr);
    LOG_ERROR("%s", buffer);

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
