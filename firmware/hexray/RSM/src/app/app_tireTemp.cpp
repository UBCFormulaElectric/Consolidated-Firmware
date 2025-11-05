#include "app_tireTemp.hpp"
#include "io_tireTemp.hpp"

extern "C"
{
    #include "app_canTx.h"
}

#define MIN_TIRE_TEMPERATURE_CELSIUS -20.0f
#define MAX_TIRE_TEMPERATURE_CELSIUS 200.0f

namespace app::tireTemp
{    
    void broadcast()
    {
        const float temperature = io::tireTemp::get();
    
        const bool outOfRange = (temperature >= MAX_TIRE_TEMPERATURE_CELSIUS) || (temperature <= MIN_TIRE_TEMPERATURE_CELSIUS);

        if (outOfRange == true)
        {
            app_canTx_RSM_Info_TireTemperatureOutOfRange_set(outOfRange);
        }
        else
        {
            app_canTx_RSM_TireTemperature_set(temperature);
        }
    }
}