#include "io_brake.hpp"
<<<<<<< HEAD
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

#define BRAKE_PRESSURE_OC_THRESHOLD_V (0.33f) // Under-voltage threshold (indicative of open circuit)
#define BRAKE_PRESSURE_SC_THRESHOLD_V (3.0f)  // Over-voltage threshold (indicative of short circuit)
#define BRAKE_ACTUATED_THRESHOLD_PSI (200.0f) // Minimum pressure (in Psi) to consider the brake actuated

// Maximum measurable pressure (in Psi) over the sensor's operating range
// Psi per Volt: (Max Pressure - Min Pressure) / (Max Input Voltage - Min Input Voltage)
// https://www.cdiweb.com/products/detail/m3031000005100pg-te-connectivity/327606
#define BRAKE_PSI_PER_VOLT (2500.0f / (BRAKE_PRESSURE_SC_THRESHOLD_V - BRAKE_PRESSURE_OC_THRESHOLD_V))

namespace io::brake
{
// Converts an ADC voltage reading to brake pressure (in Psi)
static float pressureFromVoltage(float voltage)
{
    // The sensor's effective input range is reduced by the voltage divider.
    // Calculation: (ADC Voltage - Minimum Input Voltage) * Psi Per Volt
    return BRAKE_PSI_PER_VOLT * (voltage - BRAKE_PRESSURE_OC_THRESHOLD_V);
}

bool isActuated(void)
{
    return io::brake::getFrontPressurePsi() > BRAKE_ACTUATED_THRESHOLD_PSI;
=======
// #include "hw_adcs.hpp"
// #include "hw_gpios.hpp"

// Just some dummy functions to pass the Github build checks, NEEDED TO IMPLEMENT THESE FUNCTIONS LATER
namespace io::brake
{
bool isActuated(void)
{
    return false;
>>>>>>> master
}

float getFrontPressurePsi(void)
{
<<<<<<< HEAD
    return pressureFromVoltage(hw::adcs::bps_f.getVoltage());
=======
    return 0;
>>>>>>> master
}

bool OCSC(void)
{
<<<<<<< HEAD
    return !hw::gpios::nbps_f_ocsc.readPin();
}
} // namespace io::brake
=======
    return false;
}
} // namespace io::brake
>>>>>>> master
