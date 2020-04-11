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
void SetBatteryVoltage(
    float battery_voltage); // This is the original function which we are
                            // generated stubs for
void SetBatteryVoltage_override_hook(
    float
        battery_voltage); // This is the function that the user should overwrite

static void SetBatteryVoltage_normalSetter(float battery_voltage);
static void SetBatteryVoltage_overrideSetter(float battery_voltage);
static void SetBatteryVoltage_stopOverride(void);

struct setter SetBatteryVoltage_setter = {
    .normal_setter       = SetBatteryVoltage_normalSetter,
    .override_setter     = SetBatteryVoltage_overrideSetter,
    .disable_override    = SetBatteryVoltage_stopOverride,
    .is_override_enabled = false,
};

/*
 * @info Setter function to indicate whether override is in
 *       progress for SetBatteryVoltage
 */
static void SetBatteryVoltage_stopOverride(void)
{
    SetBatteryVoltage_setter.is_override_enabled = false;
}

/**
 * @info Wrapper for the SetBatteryVoltage to only prohibit from being
 *       called when an override is in progress
 */
static void SetBatteryVoltage_normalSetter(float battery_voltage)
{
    if (SetBatteryVoltage_setter.is_override_enabled == false)
    {
        SetBatteryVoltage(battery_voltage);
    }
}

/**
 * @info Override function
 */
static void SetBatteryVoltage_overrideSetter(float battery_voltage)
{
    SetBatteryVoltage_setter.is_override_enabled = true;

    if (SetBatteryVoltage_setter.is_override_enabled == true)
    {
        SetBatteryVoltage(battery_voltage);
    }
}
