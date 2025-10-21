#include "io/thermistors.h"
#include "hw/gpios.h"
#include "hw/adcs.h"
#include "app/thermistor.h"

#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.3f)
#define SIZE_OF_TEMPERATURE_LUT (21U)

float lut_resistances[SIZE_OF_TEMPERATURE_LUT] = { 32650.0f, 25390.0f, 19900.0f, 15710.0f, 12490.0f, 10000.0f, 8057.0f,
                                                   6531.0f,  5327.0f,  4369.0f,  3603.0f,  2986.0f,  2488.0f,  2083.0f,
                                                   1752.0f,  1481.0f,  1258.0f,  1072.0f,  917.7f,   788.5f,   680.0f };

// A 0-100°C temperature reverse lookup table with 5°C resolution for a
// B57861S0103A039 thermistor. The 0th index represents 0°C. Incrementing the
// index represents a 5°C increase in temperature.
const ThermistorLUT b57861s_lut = {
    .starting_temp = 0U,
    .resolution    = 5U,
    .size          = SIZE_OF_TEMPERATURE_LUT,
    .resistances   = lut_resistances,
};

bool io_thermistors_muxSelect(const uint8_t channel)
{
    if (channel > 7)
    {
        return false;
    }

    hw_gpio_writePin(&tsense_sel0_pin, (channel >> 0) & 0x1);
    hw_gpio_writePin(&tsense_sel1_pin, (channel >> 1) & 0x1);
    hw_gpio_writePin(&tsense_sel2_pin, (channel >> 2) & 0x1);

    return true;
}

float io_thermistors_readSelectedTemp(void)
{
    const float raw_voltage           = hw_adc_getVoltage(&aux_tsns);
    const float thermistor_resistance = (raw_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - raw_voltage);

    return app_thermistor_resistanceToTemp(thermistor_resistance, &b57861s_lut);
}
