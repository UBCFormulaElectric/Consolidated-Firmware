#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adcs.h"

// CHECK MIN/MAX VALUES FOR RSM

#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

namespace io::suspension
{

static const SuspensionSensor right();
static const SuspensionSensor left();

float SuspensionSensor::getTravel()
{
    return src.getVoltage();
}

float SuspensionSensor::OCSC()
{
    float sensorVoltage = getTravel();
    return !(MIN_SUSPENSION_VOLTAGE <= sensorVoltage && sensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}


} // namespace io::suspension