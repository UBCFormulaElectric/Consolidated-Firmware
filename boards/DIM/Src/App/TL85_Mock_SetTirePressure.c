/* This file is auto-generated. DO NOT MODIFY! */

#include "TL85.h"
#include "stdbool.h"

#define WEAK_FUNC __attribute__((weak))
#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

/* Provide function declarations manually for the helper functions: This may
   seem "hacky", but the alternative would be to include header files like
   adc_readings.h in TL85.h, and that makes this library more
   diffuclt to use */
void SetTirePressure(float tire_pressure); // This is the original function
                                           // which we are generated stubs for
void SetTirePressure_override_hook(
    float tire_pressure); // This is the function that the user should overwrite

static void SetTirePressure_normalSetter(float tire_pressure);
static void SetTirePressure_overrideSetter(float tire_pressure);
static void SetTirePressure_stopOverride(void);

struct setter SetTirePressure_setter = {
    .normal_setter       = SetTirePressure_normalSetter,
    .override_setter     = SetTirePressure_overrideSetter,
    .disable_override    = SetTirePressure_stopOverride,
    .is_override_enabled = false,
};

/*
 * @info Setter function to indicate whether override is in
 *       progress for SetTirePressure
 */
static void SetTirePressure_stopOverride(void)
{
    SetTirePressure_setter.is_override_enabled = false;
}

/**
 * @info Wrapper for the SetTirePressure to only prohibit from being
 *       called when an override is in progress
 */
static void SetTirePressure_normalSetter(float tire_pressure)
{
    if (SetTirePressure_setter.is_override_enabled == false)
    {
        SetTirePressure(tire_pressure);
    }
}

/**
 * @info Override function
 */
static void SetTirePressure_overrideSetter(float tire_pressure)
{
    SetTirePressure_setter.is_override_enabled = true;

    if (SetTirePressure_setter.is_override_enabled == true)
    {
        SetTirePressure(tire_pressure);
    }
}
