#include "io_brake.h"
#include "hw_adcs.h"
#include "hw_gpios.h"

#define PRESSURE_SPAN_PSI (1000.0f)
#define VOLTAGE_OFFSET (0.5f)
#define POTENTIAL_SPAN_VOLTS (4.5f - VOLTAGE_OFFSET)
#define VOLTS_PER_PSI (POTENTIAL_SPAN_VOLTS / PRESSURE_SPAN_PSI)

float io_brake_getPressure(void)
{
    float voltage = hw_adc_getVoltage(&bps_3v3);
    return (voltage - VOLTAGE_OFFSET) / VOLTS_PER_PSI;
}



