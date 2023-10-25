#include <math.h>
#include "Io_AcceleratorPedals.h"
#include "Io_Adc.h"
#include "App_SharedMacros.h"

// Constant used to compute gamma (cosine law)
#define PAPPS_LEN_A (175.0f) // pedal length to PAPPS
#define SAPPS_LEN_A (165.0f) // pedal length to SAPPS
#define LEN_B (90.61f)       // horizontal distance to PAPPS/ SAPPS

#define MAX_POT_LENGTH (220.0f)
#define MIN_POT_LENGTH (145.0f)

#define MAX_POT_V (3.30f)

// Raw voltage seen at the pin (used to calculate rest and max PAPPS/SAPPS length)
#define PAPPS_FULL_PRESSED_POT_V (2.61f)
#define PAPPS_UNPRESSED_POT_V (0.72f)

#define SAPPS_FULL_PRESSED_POT_V (3.12f)
#define SAPPS_UNPRESSED_POT_V (1.23f)

// Denominator term for angle calculation using the cosine law
#define PAPPS_COS_LAW_DENOMINATOR (2.0f * PAPPS_LEN_A * LEN_B)
#define PAPPS_COS_LAW_COEFFICIENT ((PAPPS_LEN_A * PAPPS_LEN_A + LEN_B * LEN_B) / PAPPS_COS_LAW_DENOMINATOR)

#define SAPPS_COS_LAW_DENOMINATOR (2.0f * SAPPS_LEN_A * LEN_B)
#define SAPPS_COS_LAW_COEFFICIENT ((SAPPS_LEN_A * SAPPS_LEN_A + LEN_B * LEN_B) / SAPPS_COS_LAW_DENOMINATOR)

// Macro to calculate the linear change in PAPPS length per change in voltage (we only use max because min = 0)
#define APPS_VOLTAGE_PER_MM ((MAX_POT_LENGTH - MIN_POT_LENGTH) / MAX_POT_V)

// linear equation to calculate current length using a rate of change from potientiometer
// here max length here is the Yint and apps voltager per mm is the slope
#define RAW_VOLTAGE_TO_LEN_MM(voltage) (MAX_POT_LENGTH - voltage * APPS_VOLTAGE_PER_MM)

// calculating the unpressed and pressed PAPPS/SAPPS length based on the measured pot readings
#define PAPPS_LENGTH_UNPRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(PAPPS_UNPRESSED_POT_V))
#define PAPPS_LENGTH_FULLY_PRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(PAPPS_FULL_PRESSED_POT_V))

#define SAPPS_LENGTH_UNPRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(SAPPS_UNPRESSED_POT_V))
#define SAPPS_LENGTH_FULLY_PRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(SAPPS_FULL_PRESSED_POT_V))

// OC & SC bounds
#define PAPPS_MIN_V (PAPPS_UNPRESSED_POT_V - 0.5f)
#define PAPPS_MAX_V (PAPPS_FULL_PRESSED_POT_V + 0.5f)

#define SAPPS_MIN_V (SAPPS_UNPRESSED_POT_V - 0.5f)
#define SAPPS_MAX_V (SAPPS_FULL_PRESSED_POT_V + 0.5f)

#define DEAD_ZONE_PERCENT (10.3f)

static float calcAppsAngle(float cos_law_coefficent, float cos_law_denominator, float pot_len)
{
    float angle = (acosf(cos_law_coefficent - (powf(pot_len, 2) / cos_law_denominator)));

    return angle;
}

float Io_AcceleratorPedals_GetPapps(void)
{
    float pedal_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_2);

    // Length calculated via voltage reading from PAPPS
    const float pot_len_mm = RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);

    const float rest_angle =
        calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_COS_LAW_DENOMINATOR, PAPPS_LENGTH_UNPRESSED_MM);

    float pedal_angle = rest_angle - calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_COS_LAW_DENOMINATOR, pot_len_mm);

    const float max_angle =
        rest_angle - calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_COS_LAW_DENOMINATOR, PAPPS_LENGTH_FULLY_PRESSED_MM);

    float pedal_percentage_raw = (pedal_angle / max_angle) * 100.0f;

    if (pedal_percentage_raw <= DEAD_ZONE_PERCENT)
    {
        return 0;
    }

    float pedal_percentage = (100 / (100 - DEAD_ZONE_PERCENT)) * (pedal_percentage_raw - DEAD_ZONE_PERCENT);

    return CLAMP(pedal_percentage, 0.0f, 100.0f);
}

bool Io_AcceleratorPedals_PappsOCSC(void)
{
    float pedal_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_2);

    return !(PAPPS_MIN_V <= pedal_voltage && pedal_voltage <= PAPPS_MAX_V);
}

float Io_AcceleratorPedals_GetSapps(void)
{
    float pedal_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_1);

    // length calc from SAPPS voltage reading
    const float pot_len_mm = RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);

    const float rest_angle =
        calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_COS_LAW_DENOMINATOR, SAPPS_LENGTH_UNPRESSED_MM);

    float pedal_angle = rest_angle - calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_COS_LAW_DENOMINATOR, pot_len_mm);

    const float max_angle =
        rest_angle - calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_COS_LAW_DENOMINATOR, SAPPS_LENGTH_FULLY_PRESSED_MM);

    float pedal_percentage_raw = (pedal_angle / max_angle) * 100.0f;

    if (pedal_percentage_raw <= DEAD_ZONE_PERCENT)
    {
        return 0;
    }

    float pedal_percentage = (100 / (100 - DEAD_ZONE_PERCENT)) * (pedal_percentage_raw - DEAD_ZONE_PERCENT);

    return CLAMP(pedal_percentage, 0.0f, 100.0f);
}

bool Io_AcceleratorPedals_SappsOCSC(void)
{
    float pedal_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_1);
    return !(SAPPS_MIN_V <= pedal_voltage && pedal_voltage <= SAPPS_MAX_V);
}