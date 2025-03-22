#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adc.h"

// Voltage thresholds for the suspension sensor (in Volts)
#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

static const SuspensionConfig *config = NULL;

void io_suspension_init(const SuspensionConfig *suspension_config)
{
    config = suspension_config;
}

float io_suspension_getLeftTravel(void)
{
    return hw_adc_getVoltage(config->front_left_suspension); // Return left suspension travel based on the ADC voltage.
}

float io_suspension_getRightTravel(void)
{
    return hw_adc_getVoltage(
        config->front_right_suspension); // Return right suspension travel based on the ADC voltage.
}

bool io_suspension_leftSensorOCSC(void)
{
    float leftSensorVoltage = hw_adc_getVoltage(config->front_left_suspension);
    return !(
        MIN_SUSPENSION_VOLTAGE <= leftSensorVoltage &&
        leftSensorVoltage <= MAX_SUSPENSION_VOLTAGE); // Check if the left sensor voltage is within the valid range.
}

bool io_suspension_rightSensorOCSC(void)
{
    float rightSensorVoltage = hw_adc_getVoltage(config->front_right_suspension);
    return !(
        MIN_SUSPENSION_VOLTAGE <= rightSensorVoltage &&
        rightSensorVoltage <= MAX_SUSPENSION_VOLTAGE); // Check if the right sensor voltage is within the valid range.
}
