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

/*
float getRearLeftTravel(void)
{
    return susp_travel_rl_3v3.getVoltage();
}

float getRearRightTravel(void)
{
    return susp_travel_rr_3v3.getVoltage();
}

bool leftSensorOCSC(void)
{
    float leftSensorVoltage = susp_travel_rl_3v3.getVoltage();
    return !(MIN_SUSPENSION_VOLTAGE <= leftSensorVoltage && leftSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}

bool rightSensorOCSC(void)
{
    float rightSensorVoltage = susp_travel_rr_3v3.getVoltage();
    return !(MIN_SUSPENSION_VOLTAGE <= rightSensorVoltage && rightSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}
*/

} // namespace io::suspension