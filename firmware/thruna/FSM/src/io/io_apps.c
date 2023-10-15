#include "io_apps.h"
#include <tgmath.h>
#include "hw_adc.h"
#include "App_SharedMacros.h"

// Constant used to compute gamma (cosine law)
#define PAPPS_LEN_A (175.0f) // pedal length to PAPPS
#define SAPPS_LEN_A (165.0f) // pedal length to SAPPS
#define LEN_B (90.61f)       // horizontal distance to PAPPS/ SAPPS

#define MAX_POT_LENGTH (220.0f)
#define MIN_POT_LENGTH (145.0f)

#define MAX_POT_V (3.30f)
// min pot voltage is 0

// Raw voltage seen at the pin (used to calculate rest and max PAPPS/SAPPS length)
#define PAPPS_FULL_PRESSED_POT_V (2.57f)
#define PAPPS_UNPRESSED_POT_V (0.86f)

#define SAPPS_FULL_PRESSED_POT_V (3.08f)
#define SAPPS_UNPRESSED_POT_V (1.38f)

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

#define DEAD_ZONE_PERCENT (10.0f)

static float papps_rest_angle;
static float papps_max_angle;

static float sapps_rest_angle;
static float sapps_max_angle;

// max and min angle calculation for PAPPS/SAPPS
static float calcAppsAngle(float cos_law_coefficent, float pot_len, float cos_law_denominator)
{
    // clamping input of acos from cos law ((a^2 + b^2 - c^2) / 2ab))
    float acosf_input = CLAMP((cos_law_coefficent - (powf(pot_len, 2) / cos_law_denominator)), -1.0f, 1.0f);

    float angle = acosf(acosf_input);

    return angle;
}

void io_apps_init(void)
{
    papps_rest_angle = calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_LENGTH_UNPRESSED_MM, PAPPS_COS_LAW_DENOMINATOR);
    papps_max_angle =
        papps_rest_angle -
        calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_LENGTH_FULLY_PRESSED_MM, PAPPS_COS_LAW_DENOMINATOR);

    sapps_rest_angle = calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_LENGTH_UNPRESSED_MM, SAPPS_COS_LAW_DENOMINATOR);
    sapps_max_angle =
        sapps_rest_angle -
        calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_LENGTH_FULLY_PRESSED_MM, SAPPS_COS_LAW_DENOMINATOR);
}

float io_apps_getPrimary(void)
{
    // Length calculated via voltage
    float pedal_voltage = hw_adc_getVoltage(ADC1_CHANNEL_2);

    // Length calculated via voltage reading from PAPPS
    const float pot_len_mm = RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);

    float pedal_angle =
        papps_rest_angle - calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, pot_len_mm, PAPPS_COS_LAW_DENOMINATOR);

    float pedal_percentage_raw = (pedal_angle / papps_max_angle) * 100.0f;

    if (pedal_percentage_raw <= DEAD_ZONE_PERCENT)
    {
        return 0;
    }

    float pedal_percentage = (100 / (100 - DEAD_ZONE_PERCENT)) * (pedal_percentage_raw - DEAD_ZONE_PERCENT);

    return CLAMP(pedal_percentage, 0.0f, 100.0f);
}

bool io_apps_isPrimaryOCSC(void)
{
    float pedal_voltage = hw_adc_getVoltage(ADC1_CHANNEL_2);

    return !(PAPPS_MIN_V <= pedal_voltage && pedal_voltage <= PAPPS_MAX_V);
}

float io_apps_getSecondary(void)
{
    // Length calculated via voltage
    float pedal_voltage = hw_adc_getVoltage(ADC1_CHANNEL_1);

    // length calc from SAPPS voltage reading
    const float pot_len_mm = RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);

    float pedal_angle_raw = calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, pot_len_mm, SAPPS_COS_LAW_DENOMINATOR);

    float pedal_angle =
        sapps_rest_angle - calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, pot_len_mm, SAPPS_COS_LAW_DENOMINATOR);

    float pedal_percentage_raw = (pedal_angle / sapps_max_angle) * 100.0f;

    if (pedal_percentage_raw <= DEAD_ZONE_PERCENT)
    {
        return 0;
    }

    float pedal_percentage = (100 / (100 - DEAD_ZONE_PERCENT)) * (pedal_percentage_raw - DEAD_ZONE_PERCENT);

    return CLAMP(pedal_percentage, 0.0f, 100.0f);
}

bool io_apps_isSecondaryOCSC(void)
{
    float pedal_voltage = hw_adc_getVoltage(ADC1_CHANNEL_1);

    return !(SAPPS_MIN_V <= pedal_voltage && pedal_voltage <= SAPPS_MAX_V);
}