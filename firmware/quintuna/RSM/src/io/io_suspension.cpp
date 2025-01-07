#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adcs.h"

// CHECK MIN/MAX VALUES FOR RSM

#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

static const SuspensionSensor right = {coolantpressure1_3v3};
static const SuspensionSensor left = {coolantpressure2_3v3};

float io_suspension_getTravel(SuspensionSensor *suspension)
{
    return src.getVoltage(&suspension -> src);
}

float io_suspension_OCSC(SuspensionSensor *suspension)
{
    float sensorVoltage = io_suspension_getTravel(suspension);
    return !(MIN_SUSPENSION_VOLTAGE <= sensorVoltage && sensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}
