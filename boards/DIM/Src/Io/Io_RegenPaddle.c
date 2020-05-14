#include "Io_RegenPaddle.h"
#include "Io_Adc.h"

uint32_t Io_RegenPaddle_GetPaddlePosition(void)
{
    // 0V maps to 0% pressed and 3.3V maps to 100% pressed
    return (uint32_t)(100.0f * Io_Adc_GetChannel12Voltage() / 3.3f);
}
