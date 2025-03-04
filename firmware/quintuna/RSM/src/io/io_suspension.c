#include "io_suspension.h"
#include "hw_adcs.h"

// CHECK MIN/MAX VALUES FOR RSM

#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

const Suspension right = { .src = &susp_travel_rr_3v3 };
const Suspension left  = { .src = &susp_travel_rl_3v3 };

float io_suspension_travelFromVoltage(float voltage)
{
    // TODO: implement
    return voltage;
} 

float io_suspension_getTravel(const Suspension *suspension)
{
    return hw_adc_getVoltage(suspension->src);
}

bool io_suspension_OCSC(const Suspension *suspension)
{
    float sensorVoltage = hw_adc_getVoltage(suspension->src);
    return !(MIN_SUSPENSION_VOLTAGE <= sensorVoltage && sensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}