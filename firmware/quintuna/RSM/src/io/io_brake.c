#include "io_brake.h"
#include "hw_adcs.h"

// https://www.cdiweb.com/products/detail/m3031000005100pg-te-connectivity/327606/
#define PRESSURE_SPAN_PSI (100.0f)
#define VOLTAGE_OFFSET (0.5f)
#define MAX_VOLTAGE (4.5f)
#define PSI_PER_VOLT (PRESSURE_SPAN_PSI / (MAX_VOLTAGE - VOLTAGE_OFFSET))

float io_brake_getPressure(void)
{
    const float voltage = hw_adc_getVoltage(&bps_3v3);
    return (voltage - VOLTAGE_OFFSET) * PSI_PER_VOLT;
}
