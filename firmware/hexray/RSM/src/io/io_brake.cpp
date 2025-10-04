#include "io_brake.hpp"
#include "hw_adc.hpp"

#define BRAKE_PRESSURE_OC_THRESHOLD_V 0.33f //Change if under-voltage threshold is diff
#define BRAKE_PRESSURE_SC_THRESHOLD_V 3.0f //Change if over-voltage threshold is diff
#define BRAKE_ACTUATED_THRESHOLD_PSI 200.0f //Change if min pressure for brake to be considered actuated is diff


//Max measurable pressure over sensors operating range
// Psi per Volt: (Max Pressure - Min Pressure) / (Max Input Voltage - Min Input Voltage)
//QUESTION: Why does 2500.0 = max - min pressure
#define BRAKE_PSI_PER_VOLT (2500.0f / (BRAKE_PRESSURE_SC_THRESHOLD_V - BRAKE_PRESSURE_OC_THRESHOLD_V))


static float pressureFromVoltage(float voltage)
{
    //Sensor's effective input range is reduced by the voltage threshold??
    //Calculation: (ADC Voltage - Min Input Voltage) *PSI per volt
    return (BRAKE_PSI_PER_VOLT * (voltage - BRAKE_PRESSURE_OC_THRESHOLD_V));
}

bool io_brake_isActuated()
{   
    //Brake is actuated when Pressure is greater than threshold
    return io_brake_getRearPressurePsi() > BRAKE_ACTUATED_THRESHOLD_PSI;
}

float io_brake_getRearPressurePsi()
{
    //function and address of bps_3v3 not initialized yet
    return pressureFromVoltage(hw_adc_getVoltage(&bps_3v3));
}

// bool io_brake_OCSC()
// {   
//     //hw_gpio_readPin and &nbps_f_ocsc not initialized yet
//     return !hw_gpio_readPin(&nbps_f_ocsc);
// }