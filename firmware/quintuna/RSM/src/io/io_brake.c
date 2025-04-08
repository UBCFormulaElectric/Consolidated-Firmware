#include "io_brake.h"
#include "hw_adcs.h"
#include "hw_gpios.h"

#define PRESSURE_SPAN_PSI (1000.0f)
#define VOLTAGE_OFFSET (0.5f)
#define POTENTIAL_SPAN_VOLTS (4.5f - VOLTAGE_OFFSET)
#define VOLTS_PER_PSI (POTENTIAL_SPAN_VOLTS / PRESSURE_SPAN_PSI)

float io_brake_getPressure(void)
{
    float voltage = hw_adc_getVoltage(&bps_3v3);
    return (voltage - VOLTAGE_OFFSET) / VOLTS_PER_PSI;
}



/*
def brake_pressure_to_potential_volts(brake_pressure_psi: float) -> float:
    """Convert from brake pressure to pressure sensor voltage output.

    Args:
        brake_pressure_psi: Target pressure in PSI.

    Returns:
        Output voltage of pressure sensor in volts.

    """

    # From direct charecterization of sensors.
    pressure_span_psi = 1000
    voltage_offset = 0.5
    potential_span_volts = 4.5 - voltage_offset
    volts_per_psi = potential_span_volts / pressure_span_psi

    return voltage_offset + volts_per_psi * brake_pressure_psi
*/