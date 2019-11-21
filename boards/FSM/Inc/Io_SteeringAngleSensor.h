#ifndef FSM_IO_STEERING_ANGLE_SENSOR_H
#define FSM_IO_STEERING_ANGLE_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

// TODO: block at the top here with usage instructions?

// TODO: prefix all enum values with enumname?
enum Io_SteeringAngleSensor_Status
{
    // Sensor is in a good state
    OK,
    // Sensor is reading value above expected maximum
    ABOVE_MAX_EXPECTED_ANGLE,
    // Sensor is reading value below expected minimum
    BELOW_MIN_EXPECTED_ANGLE,
    // `init` method has not been called yet
    NOT_INITIALIZED,
};

/**
 * Setup the steering angle sensor
 *
 * @param steering_angle_adc_rank The rank in the ADC the steering angle
 *                                sensor is connected to
 * @param max_expected_angle_radians The maximum expected angle the steering
 * angle sensor should be expected to read
 * @param min_expected_angle_radians The minimum expected angle the steering
 * angle sensor should be expected to read
 * @param max_angle_voltage The voltage corresponding to the
 * maximum expected steering_angle
 * @param min_angle_voltage The voltage corresponding to the
 * minimum expected steering_angle
 *
 * @return true if initialization succeeded, false otherwise
 */
bool Io_SteeringAngleSensor_init(
    int   steering_angle_adc_rank,
    float max_expected_angle_radians,
    float min_expected_angle_radians,
    float max_angle_voltage,
    float min_angle_voltage);

// TODO: better name and jdoc comment
void Io_SteeringAngleSensor_rawGpioReading(uint32_t reading);

/**
 * Get the current steering angle
 *
 * @param steering_angle_radians A pointer to a float that will be set to the
 * current steering angle, in radians
 *
 * @return A status indicating if the read was successful and the if the data is
 *         valid
 */
enum Io_SteeringAngleSensor_Status
    Io_SteeringAngleSensor_getCurrentSteeringAngle(
    float *steering_angle_radians);

#endif // FSM_IO_STEERING_ANGLE_SENSOR_H
