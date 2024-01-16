#include "app_thermistors.h"
#include "App_CanTx.h"

#define NUM_AUX_THERMISTORS (6U)

uint8_t thermistor_channel;
float   therm_temps[NUM_AUX_THERMISTORS];

void app_thermistors_init()
{
    thermistor_channel = 0;

    for (uint8_t i = 0; i < NUM_AUX_THERMISTORS; i++)
    {
        therm_temps[i] = 0.0f;
    }
}

void app_thermistors_updateAuxThermistorTemps()
{
    therm_temps[thermistor_channel] = io_thermistors_readSelectedTemp();

    thermistor_channel++;
    thermistor_channel %= NUM_AUX_THERMISTORS;

    io_thermistors_muxSelect(thermistor_channel);
}

void app_thermistors_broadcast()
{
    App_CanTx_BMS_ThermTemp0_Set(therm_temps[0]);
    App_CanTx_BMS_ThermTemp1_Set(therm_temps[1]);
    App_CanTx_BMS_ThermTemp2_Set(therm_temps[2]);
    App_CanTx_BMS_ThermTemp3_Set(therm_temps[3]);
    App_CanTx_BMS_ThermTemp4_Set(therm_temps[4]);
    App_CanTx_BMS_ThermTemp5_Set(therm_temps[5]);
}
