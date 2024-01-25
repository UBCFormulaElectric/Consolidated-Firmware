#include "io_thermistors.h"
#include "hw_hal.h"

#define SIZE_OF_TEMPERATURE_LUT (201U)
#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.3f)
#define THERM_INDEX_TO_DEGC (0.5f)
#define THERM_LOOKUP_STARTING_TEMP (255.0f)

static const ThermistorsConfig* config;

// A 0-100°C temperature reverse lookup table with 0.5°C resolution for a Mitsubishi
// DTN-V103J3T-DGS103V thermistor. The 0th index represents 0°C. Incrementing the
// index represents a 0.5°C increase in temperature.
static const float temp_resistance_lut[SIZE_OF_TEMPERATURE_LUT] = {
    32624.2f, 31804.3f, 31007.3f, 30232.8f, 29479.9f, 28747.9f, 28036.3f, 27344.5f, 26671.8f, 26017.6f, 25381.4f,
    24762.6f, 24160.7f, 23575.3f, 23005.7f, 22451.6f, 21912.4f, 21387.8f, 20877.3f, 20380.5f, 19896.9f, 19426.2f,
    18968.0f, 18522.0f, 18087.8f, 17664.9f, 17253.2f, 16852.3f, 16461.9f, 16081.6f, 15711.3f, 15350.5f, 14999.0f,
    14656.6f, 14323.0f, 13998.0f, 13681.2f, 13372.6f, 13071.7f, 12778.5f, 12492.8f, 12214.2f, 11942.6f, 11677.8f,
    11419.7f, 11168.0f, 10922.5f, 10683.2f, 10449.8f, 10222.0f, 10000.0f, 9783.4f,  9572.1f,  9365.9f,  9164.7f,
    8968.5f,  8777.0f,  8590.1f,  8407.7f,  8229.7f,  8056.0f,  7886.4f,  7720.8f,  7559.2f,  7401.4f,  7247.4f,
    7097.0f,  6950.1f,  6806.6f,  6666.6f,  6529.7f,  6396.1f,  6265.6f,  6138.1f,  6013.5f,  5891.8f,  5772.9f,
    5656.7f,  5543.2f,  5432.3f,  5323.9f,  5217.9f,  5114.4f,  5013.2f,  4914.2f,  4817.5f,  4722.9f,  4630.5f,
    4540.1f,  4451.7f,  4365.3f,  4280.8f,  4198.1f,  4117.3f,  4038.2f,  3960.9f,  3885.2f,  3811.2f,  3738.8f,
    3668.0f,  3598.7f,  3530.9f,  3464.6f,  3399.7f,  3336.1f,  3273.9f,  3213.1f,  3153.5f,  3095.2f,  3038.1f,
    2982.3f,  2927.6f,  2874.0f,  2821.6f,  2770.3f,  2720.0f,  2670.8f,  2622.6f,  2575.3f,  2529.1f,  2483.8f,
    2439.5f,  2396.0f,  2353.4f,  2311.7f,  2270.9f,  2230.9f,  2191.6f,  2153.2f,  2115.6f,  2078.7f,  2042.5f,
    2007.1f,  1972.3f,  1938.3f,  1904.9f,  1872.2f,  1840.1f,  1808.7f,  1777.9f,  1747.7f,  1718.0f,  1689.0f,
    1660.5f,  1632.6f,  1605.2f,  1578.3f,  1552.0f,  1526.1f,  1500.8f,  1475.9f,  1451.5f,  1427.6f,  1404.2f,
    1381.1f,  1358.5f,  1336.4f,  1314.6f,  1293.3f,  1272.4f,  1251.8f
};

void io_thermistors_init(const ThermistorsConfig* therm_config)
{
    config = therm_config;
}

void io_thermistors_muxSelect(uint8_t channel)
{
    hw_gpio_writePin(&config->mux_0_gpio, channel & 1 << 0);
    hw_gpio_writePin(&config->mux_1_gpio, channel & 1 << 1);
    hw_gpio_writePin(&config->mux_2_gpio, channel & 1 << 2);
    hw_gpio_writePin(&config->mux_3_gpio, channel & 1 << 3);
}

float io_thermistors_readSelectedTemp(void)
{
    float raw_voltage = hw_adc_getVoltage(config->thermistor_adc_channel);

    // The following configuration is now the thermistor temperature is
    // calculated
    //
    // - VREF is ~3.3V provided by voltage regulation circuitry
    // - The 10kOhm resistor on top is the bias resistor used to help compute
    // the resistance across the thermistor
    //
    // (1) The thermistor resistance is first determined from the voltage drop
    // (VTemp - V-) across the thermistor (2) Using the thermistor resistance we
    // can use the look up table provided to get the temperature of the
    // thermistor
    //
    //
    //       ┌─── VREF
    //       │
    //      ┌┴┐
    //      │ │
    //      │ │ 10kOhm
    //      │ │
    //      └┬┘
    //       │----- VTemp
    //      ┌┴┐
    //      │ │
    //      │ │ DTN-V103J3T-DGS103V thermistor
    //      │ │
    //      └┬┘
    //       │
    //       └─── V-
    //
    //

    float thermistor_temp = THERM_LOOKUP_STARTING_TEMP;
    //    const float gpio_voltage          = (float)raw_thermistor_voltage * V_PER_100UV;
    const float thermistor_resistance = (raw_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - raw_voltage);

    // Check that the thermistor resistance is in range
    if ((thermistor_resistance <= temp_resistance_lut[0]) &&
        (thermistor_resistance >= temp_resistance_lut[SIZE_OF_TEMPERATURE_LUT - 1U]))
    {
        // Find the index corresponding to the calculated thermistor
        // resistance
        uint8_t therm_lut_index = 0U;
        for (therm_lut_index = 0U; thermistor_resistance < temp_resistance_lut[therm_lut_index]; therm_lut_index++)
            ;

        // Divide the index of the thermistor lookup table by 2 as the
        // temperature lookup table's key has a resolution of 0.5°C.
        // Multiply the result by 5 as we are storing the temperature
        // as in deci °C.
        //
        //                                 THERMISTOR LUT INDEX * 10
        // CELL_TEMPERATURES_DECI_DEG_C = ----------------------------
        //                                            2
        //
        thermistor_temp = therm_lut_index * THERM_INDEX_TO_DEGC;
    }

    return thermistor_temp;
}
