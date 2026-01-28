#include "io_steering.hpp"
<<<<<<< HEAD
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

// Voltage thresholds for the steering sensor (in Volts)
#define MIN_STEERING_VOLTAGE (0.2f)
#define MAX_STEERING_VOLTAGE (3.2f) // TODO: need to be validated

// Constants for steering angle calculation
#define STEERING_ANGLE_VOLTAGE_OFFSET (2.21f) // TODO: still needs to be validated
#define DEGREE_PER_VOLT (360.0f / (MAX_STEERING_VOLTAGE - MIN_STEERING_VOLTAGE))

=======

// Just some dummy functions to pass the Github build checks, NEEDED TO IMPLEMENT THESE FUNCTIONS LATER
>>>>>>> master
namespace io::steering
{
float getAngleDegrees(void)
{
<<<<<<< HEAD
    const float steering_voltage = hw::adcs::str_angle.getVoltage(); // Get the ADC voltage for the steering sensor.
    return DEGREE_PER_VOLT *
           (steering_voltage - STEERING_ANGLE_VOLTAGE_OFFSET); // Calculate and return the steering angle in degrees.
=======
    return 0;
>>>>>>> master
}

bool sensorOCSC(void)
{
<<<<<<< HEAD
    return !hw::gpios::nstr_angle_ocsc.readPin();
}
} // namespace io::steering
=======
    return false;
}
} // namespace io::steering
>>>>>>> master
