#include "Io_RegenPaddle.h"
#include "Io_Adc.h"

uint32_t Io_RegenPaddle_GetPaddlePosition(void)
{
    // TODO: Fill this in when the ADC driver is configured for regen paddle
    return (uint32_t)(100.0f * Io_Adc_GetChannel12Voltage() / 3.3f);
}
