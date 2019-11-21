#include "Io_SteeringAngleSensor.h"
#include "SharedAdc.h"

#include <stdint.h>
#include <stm32f3xx_hal.h>
#include <stdbool.h>

// The rank in the ADC the steering angle sensor is connected to
static int _steering_angle_adc_rank = 0;

// Min/max expected angles the steering angle sensor should read
static float _max_expected_angle_radians = 0.0;
static float _min_expected_angle_radians = 0.0;

// Voltages for the min/max angle the steering angle should expect to read
static uint32_t _max_angle_voltage = 0;
static uint32_t _min_angle_voltage = 0;

// Whether or not the `init` function was successfully called
bool _initialized = false;

/**
 * Convert the given voltage to a steering angle
 *
 * @param voltage The voltage to convert
 * @return The steering angle corresponding to the given voltage
 */
float convertVoltageToSteeringAngle(float voltage)
{
    return _min_expected_angle_radians +
           (_max_expected_angle_radians - _min_expected_angle_radians) /
               (_max_angle_voltage - _min_angle_voltage) * voltage;
}

bool Io_SteeringAngleSensor_init(
    int   steering_angle_adc_rank,
    float max_expected_angle_radians,
    float min_expected_angle_radians,
    float max_expected_voltage,
    float min_expected_voltage)
{
    if (steering_angle_adc_rank < 1)
    {
        return false;
    }

    _steering_angle_adc_rank    = steering_angle_adc_rank;
    _max_expected_angle_radians = max_expected_angle_radians;
    _min_expected_angle_radians = min_expected_angle_radians;
    _max_angle_voltage          = max_expected_voltage;
    _min_angle_voltage          = min_expected_voltage;

    _initialized = true;

    return true;
}

enum Io_SteeringAngleSensor_Status
    Io_SteeringAngleSensor_getCurrentSteeringAngle(
    float *steering_angle_radians)
{
    if (!_initialized)
    {
        return NOT_INITIALIZED;
    }
    else
    {
        float steering_angle_adc_voltage =
            SharedAdc_GetAdcVoltage(_steering_angle_adc_rank);
        *steering_angle_radians =
            convertVoltageToSteeringAngle(steering_angle_adc_voltage);
    }
    return OK;
}
