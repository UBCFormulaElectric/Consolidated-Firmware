#include "app_thermistors.h"
#include "io_thermistors.h"

uint8_t thermistor_channel;
float   therm_temps[NUM_AUX_THERMISTORS];

void app_thermistors_init(void)
{
    thermistor_channel = 0;

    for (uint8_t i = 0; i < NUM_AUX_THERMISTORS; i++)
    {
        therm_temps[i] = 0.0f;
    }
}

void app_thermistors_updateAuxThermistorTemps(void)
{
    therm_temps[thermistor_channel] = io_thermistors_readSelectedTemp();

    thermistor_channel++;
    thermistor_channel %= NUM_AUX_THERMISTORS;

    io_thermistors_muxSelect(thermistor_channel);
}

void app_thermistors_broadcast(void)
{
    // TODO: Re-enable if aux thermistors installed
}
