#include "io_steering.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

namespace io::steering
{
// Voltage thresholds for the steering sensor (in Volts)
constexpr float MIN_STEERING_VOLTAGE = 0.2f;
constexpr float MAX_STEERING_VOLTAGE = 3.2f; // TODO: need to be validated

// Constants for steering angle calculation
constexpr float STEERING_ANGLE_VOLTAGE_OFFSET = 2.21f; // TODO: still needs to be validated
constexpr float DEGREE_PER_VOLT = 360.0f / (MAX_STEERING_VOLTAGE - MIN_STEERING_VOLTAGE);

float getAngleDegrees(void)
{
    const float steering_voltage = hw::adcs::str_angle.getVoltage(); // Get the ADC voltage for the steering sensor.
    return DEGREE_PER_VOLT *
           (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET); // Calculate and return the steering angle in degrees.
}

bool sensorOCSC(void)
{
    return !hw::gpios::nstr_angle_ocsc.readPin();
}
} // namespace io::steering
