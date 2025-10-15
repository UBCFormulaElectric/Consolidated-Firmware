#include "app_tireTemp.hpp"
#include "io_tireTemp.hpp"

extern "C"
{
    #include "app_canTx.h"
}

#define MIN_TIRE_TEMPERATURE_CELSIUS -20.0f
#define MAX_TIRE_TEMPERATURE_CELSIUS 200.0f


void app_tireTemp_broadcast()
{
    const float temperature = io_get_tireTemp();

    const bool tireTemp_outOfRange = (temperature >= MAX_TIRE_TEMPERATURE_CELSIUS) || (temperature <= MIN_TIRE_TEMPERATURE_CELSIUS);
    
    if (tireTemp_outOfRange == true)
    {
        app_canTx_RSM_Info_TireTempOutOfRange_set(tireTemp_outOfRange);
    }
    else
    {
        app_canTx_RSM_TireTemp_set(temperature);
    }

//how to add tireTemp_set and TempOutOfRange to CAN
}