#include "io_brake.h"
#include "hw_adc.h"

// Voltage thresholds for brake pressure sensor (in Volts)
#define BRAKE_PRESSURE_OC_THRESHOLD_V (0.33f) // Under-voltage threshold (indicative of open circuit)
#define BRAKE_PRESSURE_SC_THRESHOLD_V (3.0f)  // Over-voltage threshold (indicative of short circuit)

// Maximum measurable pressure (in Psi) over the sensor's operating range
// Psi per Volt: (Max Pressure - Min Pressure) / (Max Input Voltage - Min Input Voltage)
#define BRAKE_PSI_PER_VOLT (2500.0f / (BRAKE_PRESSURE_SC_THRESHOLD_V - BRAKE_PRESSURE_OC_THRESHOLD_V))

// Unused Macros Removed:
// - BRAKE_PEDAL_MIN_VOLTAGE, BRAKE_PEDAL_MAX_VOLTAGE, and BRAKE_PRESSURE_SENSOR_MAX_V

static const BrakeConfig *config = NULL;

// Checks if a given pressure sensor voltage is out-of-spec
static bool pressureSensorOCSC(float pressure_voltage)
{
    return !(BRAKE_PRESSURE_OC_THRESHOLD_V <= pressure_voltage && pressure_voltage <= BRAKE_PRESSURE_SC_THRESHOLD_V);
}

// Converts an ADC voltage reading to brake pressure (in Psi)
static float pressureFromVoltage(float voltage)
{
    // The sensor's effective input range is reduced by the voltage divider.
    // Calculation: (ADC Voltage - Minimum Input Voltage) * Psi Per Volt
    return BRAKE_PSI_PER_VOLT * (voltage - BRAKE_PRESSURE_OC_THRESHOLD_V);
}

void io_brake_init(const BrakeConfig *brake_config)
{
    config = brake_config;
}

bool io_brake_isActuated(void)
{
    return !hw_gpio_readPin(
        config->nbspd_brake_pressed); // Brake is actuated when the brake pressed signal is low (active low)
}

float io_brake_getFrontPressurePsi(void)
{
    return pressureFromVoltage(
        hw_adc_getVoltage(config->front_brake)); // Get the front brake sensor voltage and convert to pressure in Psi
}

bool io_brake_frontPressureSensorOCSC(void)
{
    float front_pressure_voltage = hw_adc_getVoltage(config->front_brake);
    return pressureSensorOCSC(front_pressure_voltage);
}

bool io_brake_hwOCSC(void) // idk really know wot this does
{
    return hw_gpio_readPin(
        config->brake_hardware_ocsc); // Return the hardware overcurrent/short-circuit status for the brake system
}
