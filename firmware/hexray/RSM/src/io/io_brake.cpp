#include "io_brake.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"
#ifdef TARGET_EMBEDDED
#include "hw_adc.hpp"
#endif

// Max measurable pressure over sensors operating range
constexpr float BRAKE_PRESSURE_OC_THRESHOLD_V = 0.4f;
constexpr float BRAKE_PRESSURE_SC_THRESHOLD_V = 4.6f;

//  Psi per Volt: (Max Pressure - Min Pressure) / (Max Input Voltage - Min Input Voltage)
constexpr float MAX_PRESSURE = 2500;
constexpr float MIN_PRESSURE = 0;
constexpr float BRAKE_PSI_PER_VOLT =
    (MAX_PRESSURE - MIN_PRESSURE) / (BRAKE_PRESSURE_SC_THRESHOLD_V - BRAKE_PRESSURE_OC_THRESHOLD_V);

float pressureFromVoltage(float voltage)
{
    // Calculation: (ADC Voltage - Min Input Voltage) * PSI per volt
    return ((voltage - BRAKE_PRESSURE_OC_THRESHOLD_V) * BRAKE_PSI_PER_VOLT);
}

namespace io::brake
{
float getRearPressurePsi()
{
    return pressureFromVoltage(hw::adcs::bps_3v3.getVoltage());
}

bool OCSC()
{
    return !hw::gpio::brake_ocsc_ok.readPin();
}
} // namespace io::brake

// float getRearPressurePsi()
// {
//     // function and address of bps_3v3 not initialized yet
//     //  return pressureFromVoltage(hw_adc_getVoltage(&bps_3v3));
//     return 0;
// }

// bool OCSC()
// {
//     //hw_gpio_readPin and &nbps_f_ocsc not initialized yet
//     return !hw_gpio_readPin(&nbps_f_ocsc);
// }
// namespace io::brake

// static float pressureFromVoltage(float voltage)
// {
//     // Sensor's effective input range is reduced by the voltage threshold??
//     // Calculation: (ADC Voltage - Min Input Voltage) *PSI per volt
//     return (BRAKE_PSI_PER_VOLT * (voltage - BRAKE_PRESSURE_OC_THRESHOLD_V));
// }
