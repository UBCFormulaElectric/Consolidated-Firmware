#include <tgmath.h>
#include <stm32f3xx.h>
#include "main.h"
#include "Io_AcceleratorPedals.h"
#include "Io_Adc.h"

// TOOD: Change these lengths
// Constant used to compute gamma (cosine law)
#define PAPPS_LEN_A (130.0f)
#define PAPPS_LEN_B (117.0f)

// Assume that the angle range is 50 degrees
#define PAPPS_LENGTH_UNPRESSED_MM (199.0f)
#define PAPPS_LENGTH_FULL_PRESSED_MM (152.0f)

// Raw voltage seen at the pin
#define PAPPS_FULL_PRESSED_POT_V (2.84f)
#define PAPPS_UNPRESSED_POT_V (0.835f)

// TODO: re-evalutate this expression
// PAPPS angle expressed in rad
#define PAPPS_ANGLE_RANGE_RADS (50.0f * (float)M_PI / 180.0f)
#define PAPPS_ANGLE_RANGE_RADS (50.0f * (float)M_PI / 180.0f)

// Denominator term for angle calculation using the cosine law
#define PAPPS_COS_LAW_DENOMINATOR (2.0f * PAPPS_LEN_A * PAPPS_LEN_B)
#define PAPPS_COS_LAW_COEFFICIENT (PAPPS_LEN_A * PAPPS_LEN_A + PAPPS_LEN_B * PAPPS_LEN_B) / PAPPS_COS_LAW_DENOMINATOR

// Calculate the distance of the pedal from the voltage
#define PAPPS_LEN_PER_VOLTAGE \
    ((PAPPS_LENGTH_FULL_PRESSED_MM - PAPPS_LENGTH_UNPRESSED_MM) / (PAPPS_FULL_PRESSED_POT_V - PAPPS_UNPRESSED_POT_V))

// PAPPS length offset in mm
#define PAPPS_LEN_OFFSET_MM (218.57f)
#define PAPPS_RAW_VOLTAGE_TO_LEN_MM(voltage) (voltage * PAPPS_LEN_PER_VOLTAGE + PAPPS_LEN_OFFSET_MM)

bool Io_AcceleratorPedals_IsPappsEncoderAlarmActive(void)
{
    return HAL_GPIO_ReadPin(PRIMARY_APPS_ALARM_GPIO_Port, PRIMARY_APPS_ALARM_Pin) == GPIO_PIN_SET;
}

bool Io_AcceleratorPedals_IsSappsEncoderAlarmActive(void)
{
    return HAL_GPIO_ReadPin(SECONDARY_APPS_ALARM_GPIO_Port, SECONDARY_APPS_ALARM_Pin) == GPIO_PIN_SET;
}

float Io_AcceleratorPedals_GetAngle(void)
{
    // Length calculated via voltage
    const float pot_len = PAPPS_RAW_VOLTAGE_TO_LEN_MM(Io_Adc_GetChannel1Voltage());

    // Compute the angle relative to the y-axis with cosine law
    const float pedal_travel_angle =
        ((float)M_PI_2 -
         (-(PAPPS_COS_LAW_COEFFICIENT - (pot_len * pot_len / PAPPS_COS_LAW_DENOMINATOR)) + (float)M_PI_2));

    return pedal_travel_angle * 180 / (float)M_PI + 16.8f;
}

float Io_AcceleratorPedals_GetPapps(void)
{
    return Io_AcceleratorPedals_GetAngle() / 30.0f * 100.0f;
}
