#include "io_thermistors.h"
#include "hw_gpios.h"
#include "hw_adcs.h"

#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.3f)

void io_thermistors_muxSelect(const uint8_t channel)
{
    hw_gpio_writePin(&tsense_sel0_pin, channel & 1 << 0);
    hw_gpio_writePin(&tsense_sel1_pin, channel & 1 << 1);
    hw_gpio_writePin(&tsense_sel2_pin, channel & 1 << 2);
}

float io_thermistors_readSelectedTemp(void)
{
    const float raw_voltage = hw_adc_getVoltage(&aux_tsns);
}
