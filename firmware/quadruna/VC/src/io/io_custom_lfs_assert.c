#include "cmsis_os.h"
#include <assert.h>
void custom_assert(int condition)
{
    // if the condition is false, assert
    if (!condition)
    {
        // if in an RTOS context, suspend the task
        if (osKernelGetState() == osKernelRunning)
        {
            osThreadSuspend(osThreadGetId());
        }

        else
        {
            assert(0);
        }
    }
}