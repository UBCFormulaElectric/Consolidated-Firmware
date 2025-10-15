#include "app_brake.hpp"
#include "io_brake.hpp"
#include <cmath>
extern "C"
{
    #include "app_canTx.h" 
}

#define MIN_BRAKE_PRESSURE_PSI 0.0f
#define MAX_BRAKE_PRESSURE_PSI 1000.0f

namespace app::brake
{
    void broadcast()
    {
        const bool brake_pressed = io::brake::isActuated();
        const float rear_pressure = io::brake::getRearPressurePsi();

        app_canTx_RSM_BrakeActuated_set(brake_pressed); 
        app_canTx_RSM_RearBrakePressure_set((uint32_t)roundf(rear_pressure));

        app_canTx_RSM_Info_RearBrakePressureOutOfRange_set(
        (rear_pressure >= MAX_BRAKE_PRESSURE_PSI) || (rear_pressure < MIN_BRAKE_PRESSURE_PSI)); 
    }
}