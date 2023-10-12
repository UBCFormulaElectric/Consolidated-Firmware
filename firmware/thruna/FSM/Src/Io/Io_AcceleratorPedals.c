#include <math.h>
#include "Io_AcceleratorPedals.h"
#include "Io_Adc.h"
#include "App_SharedMacros.h"

// Constant used to compute gamma (cosine law)
#define PAPPS_LEN_A (175.0f) // pedal length to PAPPS
#define SAPPS_LEN_A (165.0f) // pedal length to SAPPS
#define LEN_B (90.61f)       // horizontal distance to PAPPS/ SAPPS

// Assume that the angle range is 50 degrees

#define PAPPS_LENGTH_UNPRESSED_MM (210.0f)
#define PAPPS_LENGTH_FULL_PRESSED_MM (160.0f)

#define SAPPS_LENGTH_UNPRESSED_MM (196.2f)
#define SAPPS_LENGTH_FULL_PRESSED_MM (147.17f)

// Raw voltage seen at the pin

#define PAPPS_FULL_PRESSED_POT_V (2.61f)
#define PAPPS_UNPRESSED_POT_V (0.68f)

#define SAPPS_FULL_PRESSED_POT_V (3.20f)
#define SAPPS_UNPRESSED_POT_V (1.18f)

// Denominator term for angle calculation using the cosine law
#define PAPPS_COS_LAW_DENOMINATOR (2.0f * PAPPS_LEN_A * LEN_B) // always same because it is screwed in
#define PAPPS_COS_LAW_COEFFICIENT ((PAPPS_LEN_A * PAPPS_LEN_A + LEN_B * LEN_B) / PAPPS_COS_LAW_DENOMINATOR)

#define SAPPS_COS_LAW_DENOMINATOR (2.0f * SAPPS_LEN_A * LEN_B) // always same because it is screwed in
#define SAPPS_COS_LAW_COEFFICIENT ((SAPPS_LEN_A * SAPPS_LEN_A + LEN_B * LEN_B) / SAPPS_COS_LAW_DENOMINATOR)

// Calculate the distance of the pedal from the voltage
// Macro to calculate the linear change in PAPPS length per change in voltage
#define PAPPS_LEN_PER_VOLTAGE \
    ((PAPPS_LENGTH_FULL_PRESSED_MM - PAPPS_LENGTH_UNPRESSED_MM) / (PAPPS_FULL_PRESSED_POT_V - PAPPS_UNPRESSED_POT_V))
// PAPPS length offset in mm
#define PAPPS_LEN_OFFSET_MM (PAPPS_LENGTH_UNPRESSED_MM - PAPPS_LEN_PER_VOLTAGE * PAPPS_UNPRESSED_POT_V)
#define PAPPS_RAW_VOLTAGE_TO_LEN_MM(voltage) (voltage * PAPPS_LEN_PER_VOLTAGE + PAPPS_LEN_OFFSET_MM)

#define SAPPS_LEN_PER_VOLTAGE \
    ((SAPPS_LENGTH_FULL_PRESSED_MM - SAPPS_LENGTH_UNPRESSED_MM) / (SAPPS_FULL_PRESSED_POT_V - SAPPS_UNPRESSED_POT_V))
// PAPPS length offset in mm
#define SAPPS_LEN_OFFSET_MM      \
    (SAPPS_LENGTH_UNPRESSED_MM - \
     SAPPS_LEN_PER_VOLTAGE * SAPPS_UNPRESSED_POT_V) // Yint calculated based on the measured unpressed length resulting
                                                    // in the unpressed pot val
#define SAPPS_RAW_VOLTAGE_TO_LEN_MM(voltage) (voltage * SAPPS_LEN_PER_VOLTAGE + SAPPS_LEN_OFFSET_MM)

#define PAPPS_MIN_V (PAPPS_UNPRESSED_POT_V - 0.5f)
#define PAPPS_MAX_V (PAPPS_FULL_PRESSED_POT_V + 0.5f)

#define SAPPS_MIN_V (SAPPS_UNPRESSED_POT_V - 0.5f)
#define SAPPS_MAX_V (SAPPS_FULL_PRESSED_POT_V + 0.5f)

#define DEAD_ZONE_PERCENT (10.0f)

float calcAppsAngle(float cos_law_coefficent, float cos_law_denominator, float pot_len){
    
    float angle = (acosf(cos_law_coefficent - (powf(pot_len, 2) / cos_law_denominator)));

    return angle;
}

float Io_AcceleratorPedals_GetPapps(void)
{
    float pedal_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_2);
    // Length calculated via voltage
    const float pot_len_mm = PAPPS_RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);

    const float rest_angle = calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_COS_LAW_DENOMINATOR, PAPPS_LENGTH_UNPRESSED_MM);

    float pedal_angle = rest_angle - 
                        calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_COS_LAW_DENOMINATOR, pot_len_mm);

    const float max_angle = rest_angle - 
                            calcAppsAngle(PAPPS_COS_LAW_COEFFICIENT, PAPPS_COS_LAW_DENOMINATOR, PAPPS_LENGTH_FULL_PRESSED_MM);

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
    // TODO implement IO functionality
    // Length calculated via voltage
    float       pedal_voltage = Io_Adc_GetChannelVoltage(ADC1_CHANNEL_1);
    const float pot_len_mm    = PAPPS_RAW_VOLTAGE_TO_LEN_MM(pedal_voltage);

    const float rest_angle =
        calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_COS_LAW_DENOMINATOR, SAPPS_LENGTH_UNPRESSED_MM);

    float pedal_angle = rest_angle - 
                        calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_COS_LAW_DENOMINATOR, pot_len_mm);

    const float max_angle = rest_angle - 
                            calcAppsAngle(SAPPS_COS_LAW_COEFFICIENT, SAPPS_COS_LAW_DENOMINATOR, SAPPS_LENGTH_FULL_PRESSED_MM);

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
