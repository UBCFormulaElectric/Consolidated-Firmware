#include <stdlib.h>
#include <assert.h>
#include "App_APPS.h"

struct APPS
{
    uint32_t max_count_value;
    uint32_t max_pressed_count_value;
    float    percent_deflection;

    uint32_t (*get_count_value)(void);
    bool (*is_brake_actuated)(void);
};

struct APPS *App_APPS_Create(
    uint32_t max_count_val,
    uint32_t max_pressed_count_value,
    float    percent_deflection,
    uint32_t (*get_count_value)(void),
    bool (*is_brake_actuated)(void))
{
    struct APPS *apps = malloc(sizeof(struct APPS));
    assert(apps != NULL);

    apps->max_count_value         = max_count_val;
    apps->max_pressed_count_value = max_pressed_count_value;
    apps->percent_deflection      = percent_deflection;

    apps->get_count_value   = get_count_value;
    apps->is_brake_actuated = is_brake_actuated;

    return apps;
}

float App_APPS_GetPedalPercentage(const struct APPS *const apps)
{
    const uint32_t count_value             = apps->get_count_value();
    const uint32_t max_pressed_count_value = apps->max_pressed_count_value;
    const uint32_t max_count_value         = apps->max_count_value;
    const float    percent_deflection      = apps->percent_deflection;

    // Calculate the upper threshold for the low end dead-zone
    const float low_end_deadzone_upper_threshold =
        (float)max_pressed_count_value * percent_deflection;

    // Calculate the lower threshold for the high end dead-zone
    const float high_end_deadzone_lower_threshold =
        (float)max_pressed_count_value * (1.0f - percent_deflection);

    if ((count_value > 0U && count_value <= low_end_deadzone_upper_threshold) ||
        (count_value > max_pressed_count_value &&
         count_value <= max_count_value) ||
        apps->is_brake_actuated())
    {
        // Set the pedal percentage to 0.0% to account for deflection when
        // (1) The pedal is within the depressed dead-zone
        // (2) The pedal exceeds the encoder's maximum pressed value
        // (3) The brake actuation signal is active
        return 0.0f;
    }
    else if (
        (count_value <=
         (float)max_pressed_count_value * high_end_deadzone_lower_threshold) &&
        (count_value >= max_pressed_count_value))
    {
        // Set the pedal percentage to 100% to account for deflection when the
        // pedal is within the pressed dead-zone
        return 100.0f;
    }

    // The pedal percentage is computed using the following formula:
    // PEDAL PERCENTAGE = 100% * (COUNTER VALUE - LOW END DEADZONE UPPER
    // THRESHOLD) / (HIGH END DEADZONE LOWER THRESHOLD - LOW END DEADZONE UPPER
    // THRESHOLD)
    return 100.0f * ((float)count_value - low_end_deadzone_upper_threshold) /
           (high_end_deadzone_lower_threshold -
            low_end_deadzone_upper_threshold);
}

void App_APPS_Destroy(struct APPS *apps)
{
    free(apps);
}

// TODO: Saturate the depressed range of the encoder ?
