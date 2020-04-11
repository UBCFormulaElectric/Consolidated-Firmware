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
void SetWheelSpeed(float wheel_speed); // This is the original function which we
                                       // are generated stubs for
void SetWheelSpeed_override_hook(
    float wheel_speed); // This is the function that the user should overwrite

static void SetWheelSpeed_normalSetter(float wheel_speed);
static void SetWheelSpeed_overrideSetter(float wheel_speed);
static void SetWheelSpeed_stopOverride(void);

struct setter SetWheelSpeed_setter = {
    .normal_setter       = SetWheelSpeed_normalSetter,
    .override_setter     = SetWheelSpeed_overrideSetter,
    .disable_override    = SetWheelSpeed_stopOverride,
    .is_override_enabled = false,
};

/*
 * @info Setter function to indicate whether override is in
 *       progress for SetWheelSpeed
 */
static void SetWheelSpeed_stopOverride(void)
{
    SetWheelSpeed_setter.is_override_enabled = false;
}

/**
 * @info Wrapper for the SetWheelSpeed to only prohibit from being
 *       called when an override is in progress
 */
static void SetWheelSpeed_normalSetter(float wheel_speed)
{
    if (SetWheelSpeed_setter.is_override_enabled == false)
    {
        SetWheelSpeed(wheel_speed);
    }
}

/**
 * @info Override function
 */
static void SetWheelSpeed_overrideSetter(float wheel_speed)
{
    SetWheelSpeed_setter.is_override_enabled = true;

    if (SetWheelSpeed_setter.is_override_enabled == true)
    {
        SetWheelSpeed(wheel_speed);
    }
}
