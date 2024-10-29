#include "io_brake.h"
#include "hw_adcs.h"
#include "hw_gpio.h"

const float BRAKE_PEDAL_MIN_VOLTAGE = 2.08f;
const float BRAKE_PEDAL_MAX_VOLTAGE = 2.310f;

const float BRAKE_PRESSURE_PRESSED_THRESHOLD_PSI = 40.0f;
const float BRAKE_PRESSURE_OC_THRESHOLD_V = 0.33f;
const float BRAKE_PRESSURE_SC_THRESHOLD_V = 3.0f;
const float BRAKE_PRESSURE_SENSOR_MAX_V = 5.0f;
// Psi Per Volt = (Max Pressure - Min Pressure) / (Max Input Voltage - Min Input Voltage)
const float BRAKE_PSI_PER_VOLT = 2500.0f / (BRAKE_PRESSURE_SC_THRESHOLD_V - BRAKE_PRESSURE_OC_THRESHOLD_V);

static bool pressureSensorOCSC(float pressure_voltage)
{
    return !(BRAKE_PRESSURE_OC_THRESHOLD_V <= pressure_voltage && pressure_voltage <= BRAKE_PRESSURE_SC_THRESHOLD_V);
}

static float pressureFromVoltage(float voltage)
{
    // The sensor operates from 0.5V to 4.5V. The voltage divider decreases the
    // voltage by a factor of (2/3). Thus the minimum voltage seen by the analog
    // input pin is 0.33V while the maximum voltage seen is 3V
    // Brake pressure = (ADC Voltage - Min Input Voltage) * Psi Per Volt
    return BRAKE_PSI_PER_VOLT * (voltage - BRAKE_PRESSURE_OC_THRESHOLD_V);
}

namespace io::brake
{
    bool isActuated()
    {
        return !(hw::gpio::nbspd_brake_pressed.readPin());
    }

    float io_brake_getFrontPressurePsi()
	{
    	return pressureFromVoltage(hw::adc::front_brake.getVoltage());
	}

    bool io_brake_frontPressureSensorOCSC()
	{
    	float front_pressure_voltage = hw::adc::front_brake.getVoltage();
    	return pressureSensorOCSC(front_pressure_voltage);
	}

	float io_brake_getRearPressurePsi()
	{
    	return pressureFromVoltage(hw::adc::rear_brake.getVoltage());
	}

	bool io_brake_rearPressureSensorOCSC()
	{
    	float rear_pressure_voltage = hw::adc::rear_brake.getVoltage();
    	return pressureSensorOCSC(rear_pressure_voltage);
	}

	bool io_brake_hwOCSC()
	{
    	return hw::gpio::brake_hardware_ocsc.readPin();
	}
}
