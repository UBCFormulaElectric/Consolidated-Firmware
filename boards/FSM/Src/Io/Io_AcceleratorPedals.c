#include <tgmath.h>
#include "Io_AcceleratorPedals.h"
#include "Io_Adc.h"

// TODO: Change these lengths
// Constant used to compute gamma (cosine law)
#define PAPPS_LEN_A (130.0f)
#define PAPPS_LEN_B (117.0f)
// Assume that the angle range is 50 degrees
#define PAPPS_LENGTH_UNPRESSED_MM (199.0f)
#define PAPPS_LENGTH_FULL_PRESSED_MM (152.0f)
// Raw voltage seen at the pin
#define PAPPS_FULL_PRESSED_POT_V (2.86f)
#define PAPPS_UNPRESSED_POT_V (1.082f)
// Denominator term for angle calculation using the cosine law
#define PAPPS_COS_LAW_DENOMINATOR (2.0f * PAPPS_LEN_A * PAPPS_LEN_B)
#define PAPPS_COS_LAW_COEFFICIENT (PAPPS_LEN_A * PAPPS_LEN_A + PAPPS_LEN_B * PAPPS_LEN_B) / PAPPS_COS_LAW_DENOMINATOR
// Calculate the distance of the pedal from the voltage
#define PAPPS_LEN_PER_VOLTAGE \
    ((PAPPS_LENGTH_FULL_PRESSED_MM - PAPPS_LENGTH_UNPRESSED_MM) / (PAPPS_FULL_PRESSED_POT_V - PAPPS_UNPRESSED_POT_V))
// PAPPS length offset in mm
#define PAPPS_LEN_OFFSET_MM (227.6f)
#define PAPPS_RAW_VOLTAGE_TO_LEN_MM(voltage) (voltage * PAPPS_LEN_PER_VOLTAGE + PAPPS_LEN_OFFSET_MM)

#define MIN_PEDAL_VOLTAGE (0.5f)
#define MAX_PEDAL_VOLTAGE (3.1f)

float Io_AcceleratorPedals_GetPrimaryAngle(void)
{
    // Length calculated via voltage
    float pedal_voltage = Io_Adc_GetChannel1Voltage();
    if(Io_AcceleratorPedals_PedalVoltageAlarm(pedal_voltage)){
        return NAN;
    }
    const float pot_len = PAPPS_RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);

    // Compute the angle relative to the y-axis with cosine law
    const float pedal_travel_angle =
        ((float)M_PI_2 -
         (-(PAPPS_COS_LAW_COEFFICIENT - (pot_len * pot_len / PAPPS_COS_LAW_DENOMINATOR)) + (float)M_PI_2));

    return pedal_travel_angle * 180 / (float)M_PI + 16.3f;
}
float Io_AcceleratorPedals_GetPapps(void)
{
    float primary_angle = Io_AcceleratorPedals_GetPrimaryAngle();
    if(primary_angle == NAN) return NAN;
    return Io_AcceleratorPedals_GetPrimaryAngle() / 30.0f * 100.0f;
}

//TODO Implement the secondary pedal angle IO function
float Io_AcceleratorPedals_GetSecondaryAngle(void)
{
    float pedal_voltage = 0.5f;
    if(Io_AcceleratorPedals_PedalVoltageAlarm(pedal_voltage)){
        return NAN;
    }
    return (float)M_PI;
}
float Io_AcceleratorPedals_GetSapps(void)
{
    float secondary_angle = Io_AcceleratorPedals_GetSecondaryAngle();
    if(secondary_angle == NAN) return NAN;
    return  secondary_angle / 30.0f * 100.0f;
}

bool Io_AcceleratorPedals_PedalVoltageAlarm(const float pedal_voltage)
{
    return !(MIN_PEDAL_VOLTAGE <= pedal_voltage <= MAX_PEDAL_VOLTAGE);
}
