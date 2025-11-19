#include "io_apps.hpp"
#include "hw_adcs.hpp"
#include <tgmath.h>
#include "app_utils.h"

//=====
// Geometry and ADC Constants for Pedal Sensors -> PAPPS = primary apps, SAPPS = secondary apps (same as liam's
// conventions)
//=====

// TODO: Most numbers (placeholder) are from quadruna and will need to change accordingly in each macro

// Distances from pedal pivot to sensor (in mm)
#define PAPPS_LEN_A (175.0f) // Primary sensor (PAPPS) distance
#define SAPPS_LEN_A (165.0f) // Secondary sensor (SAPPS) distance
#define LEN_B (90.61f)       // Horizontal distance to sensors

// Potentiometer length limits (in mm)
#define MAX_POT_LENGTH (220.0f)
#define MIN_POT_LENGTH (145.0f)

// ADC voltage limits (in V)
#define MAX_POT_V (3.30f)
// Note: Minimum pot voltage is assumed to be 0V

// Raw Voltage Calibration Values for PAPPS (Primary)
#define PAPPS_FULL_PRESSED_POT_V (2.57f) // Fully pressed position voltage
#define PAPPS_UNPRESSED_POT_V (0.86f)    // Unpressed position voltage

// Raw Voltage Calibration Values for SAPPS (Secondary)
#define SAPPS_FULL_PRESSED_POT_V (3.08f)
#define SAPPS_UNPRESSED_POT_V (1.38f)

// Cosine Law Pre-computed Parameters for Angle Calculation
#define PAPPS_COS_LAW_DENOMINATOR (2.0f * PAPPS_LEN_A * LEN_B) // For PAPPS sensor
#define PAPPS_COS_LAW_COEFFICIENT ((PAPPS_LEN_A * PAPPS_LEN_A + LEN_B * LEN_B) / PAPPS_COS_LAW_DENOMINATOR)

#define SAPPS_COS_LAW_DENOMINATOR (2.0f * SAPPS_LEN_A * LEN_B) // For SAPPS sensor
#define SAPPS_COS_LAW_COEFFICIENT ((SAPPS_LEN_A * SAPPS_LEN_A + LEN_B * LEN_B) / SAPPS_COS_LAW_DENOMINATOR)

// Conversion Macros
// Conversion factor from voltage to change in length (mm per volt)
#define APPS_VOLTAGE_PER_MM ((MAX_POT_LENGTH - MIN_POT_LENGTH) / MAX_POT_V)
// Convert a raw voltage reading to a potentiometer length (in mm)
#define RAW_VOLTAGE_TO_LEN_MM(voltage) (MAX_POT_LENGTH - ((voltage) * APPS_VOLTAGE_PER_MM))

// Derived calibration lengths for PAPPS and SAPPS
#define PAPPS_LENGTH_UNPRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(PAPPS_UNPRESSED_POT_V))
#define PAPPS_LENGTH_FULLY_PRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(PAPPS_FULL_PRESSED_POT_V))
#define SAPPS_LENGTH_UNPRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(SAPPS_UNPRESSED_POT_V))
#define SAPPS_LENGTH_FULLY_PRESSED_MM (RAW_VOLTAGE_TO_LEN_MM(SAPPS_FULL_PRESSED_POT_V))

// Overcurrent / Short-Circuit (OC/SC) Voltage Bounds
#define PAPPS_MIN_V (PAPPS_UNPRESSED_POT_V - 0.5f)
#define PAPPS_MAX_V (PAPPS_FULL_PRESSED_POT_V + 0.5f)
#define SAPPS_MIN_V (SAPPS_UNPRESSED_POT_V - 0.5f)
#define SAPPS_MAX_V (SAPPS_FULL_PRESSED_POT_V + 0.5f)

// Dead Zone Settings
#define DEAD_ZONE_PERCENT (10.0f) // Percentage below which the pedal is considered inactive

namespace io::apps
{
static float papps_rest_angle;
static float papps_max_angle;
static float sapps_rest_angle;
static float sapps_max_angle;

static float calcAppsAngle(const float cos_law_coefficient, const float pot_len, const float cos_law_denominator)
{
    // Calculate the cosine law expression: (a^2 + b^2 - c^2) / (2ab)
    const float value = cos_law_coefficient - pot_len * pot_len / cos_law_denominator;
    const float acos_input =
        CLAMP(value, -1.0f, 1.0f); // where c is represented indirectly via the measured length (pot_len)
    return acosf(acos_input);
}

void init()
{
    // Pre-calculate the rest (unpressed) angle and the maximum angle (difference between rest and fully pressed) for
    // each sensor.
    papps_rest_angle = calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_LENGTH_UNPRESSED_MM, PAPPS_COS_LAW_DENOMINATOR);
    papps_max_angle =
        papps_rest_angle -
        calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_LENGTH_FULLY_PRESSED_MM, PAPPS_COS_LAW_DENOMINATOR);

    sapps_rest_angle = calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_LENGTH_UNPRESSED_MM, SAPPS_COS_LAW_DENOMINATOR);
    sapps_max_angle =
        sapps_rest_angle -
        calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_LENGTH_FULLY_PRESSED_MM, SAPPS_COS_LAW_DENOMINATOR);
}

float getPrimary(void)
{
    // Read the primary sensor voltage.
    const float pedal_voltage = hw_adc_getVoltage(&apps1);
    // Convert voltage reading to a potentiometer length (in mm).
    const float pot_len_mm = RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);
    // Compute the angle difference from the rest (unpressed) position.
    const float pedal_angle =
        papps_rest_angle - calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, pot_len_mm, PAPPS_COS_LAW_DENOMINATOR);
    // Convert the angle to a raw percentage of the maximum travel.
    const float pedal_percentage_raw = (pedal_angle / papps_max_angle) * 100.0f;

    // If within the dead zone, treat as no pedal input.
    if (pedal_percentage_raw <= DEAD_ZONE_PERCENT)
    {
        return 0.0f;
    }

    // Scale the percentage to account for the dead zone.
    const float pedal_percentage = (100.0f / (100.0f - DEAD_ZONE_PERCENT)) * (pedal_percentage_raw - DEAD_ZONE_PERCENT);
    return CLAMP(pedal_percentage, 0.0f, 100.0f);
}

bool isPrimaryOCSC(void)
{
    const float pedal_voltage = hw_adc_getVoltage(&apps1);
    return !(PAPPS_MIN_V <= pedal_voltage && pedal_voltage <= PAPPS_MAX_V);
}

float getSecondary(void)
{
    // Read the secondary sensor voltage.
    const float pedal_voltage = hw_adc_getVoltage(&apps2);
    // Convert voltage reading to a potentiometer length (in mm).
    const float pot_len_mm = RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);
    // Compute the angle difference from the rest (unpressed) position.
    const float pedal_angle =
        sapps_rest_angle - calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, pot_len_mm, SAPPS_COS_LAW_DENOMINATOR);
    // Convert the angle to a raw percentage of the maximum travel.
    const float pedal_percentage_raw = (pedal_angle / sapps_max_angle) * 100.0f;

    // If within the dead zone, treat as no pedal input.
    if (pedal_percentage_raw <= DEAD_ZONE_PERCENT)
    {
        return 0.0f;
    }

    // Scale the percentage to account for the dead zone.
    const float pedal_percentage = (100.0f / (100.0f - DEAD_ZONE_PERCENT)) * (pedal_percentage_raw - DEAD_ZONE_PERCENT);
    return CLAMP(pedal_percentage, 0.0f, 100.0f);
}

bool isSecondaryOCSC(void)
{
    const float pedal_voltage = hw_adc_getVoltage(&apps2);
    return !(SAPPS_MIN_V <= pedal_voltage && pedal_voltage <= SAPPS_MAX_V);
}
} // namespace io::apps
