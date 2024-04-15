#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adc.h"

#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

SuspensionConfig *config;

void io_suspension_init(SuspensionConfig *suspension_config)
{
    config = suspension_config;
}

float travelFromVoltage(float voltage)
{
    // TODO: implement
    return voltage;
}

float io_suspension_getLeftTravel(void)
{
    return hw_adc_getVoltage(config->front_left_suspension);
}

float io_suspension_getRightTravel(void)
{
    return hw_adc_getVoltage(config->front_right_suspension);
}

bool io_suspension_leftSensorOCSC(void)
{
    float leftSensorVoltage = hw_adc_getVoltage(config->front_left_suspension);
    return !(MIN_SUSPENSION_VOLTAGE <= leftSensorVoltage && leftSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}

bool io_suspension_rightSensorOCSC(void)
{
    float rightSensorVoltage = hw_adc_getVoltage(config->front_right_suspension);
    return !(MIN_SUSPENSION_VOLTAGE <= rightSensorVoltage && rightSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}