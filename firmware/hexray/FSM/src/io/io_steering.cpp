#include "io_steering.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

namespace io::steering
{
// Voltage thresholds for the steering sensor (in Volts)
constexpr float MIN_STEERING_VOLTAGE = 0.0693040267f; // collected empirically
constexpr float MAX_STEERING_VOLTAGE = 1.47150183f;   // collected empirically
constexpr float MIN_STEERING_ANGLE   = -100.0f;       // Minimum steering angle in degrees
constexpr float MAX_STEERING_ANGLE   = 80.0f;         // Maximum steering angle in degrees

// Constants for steering angle calculation
constexpr float LOCK_TO_LOCK    = MAX_STEERING_ANGLE - MIN_STEERING_ANGLE; // Total range of steering angles
constexpr float DEGREE_PER_VOLT = LOCK_TO_LOCK / (MAX_STEERING_VOLTAGE - MIN_STEERING_VOLTAGE);

float getAngleDegrees()
{
    const float steering_voltage = str_angle.getVoltage(); // Get the ADC voltage for the steering sensor.
    return DEGREE_PER_VOLT * (steering_voltage - MIN_STEERING_VOLTAGE) +
           MIN_STEERING_ANGLE; // Calculate and return the steering angle in degrees.
}

bool sensorOCSC()
{
    return !nstr_angle_ocsc.readPin();
}
} // namespace io::steering
