#include <assert.h>
#include <math.h>
#include "Io_WheelSpeedSensors.h"
#include "Io_SharedFreqOnlyPwmInput.h"
#include "main.h"

// Note: Unit for length is measured in metres unless specified
static const float  MPS_TO_KPH_CONVERSION_FACTOR = 3.6f;
static const size_t RELUCTOR_RING_TOOTH_COUNT    = 48;
static const float  TIRE_DIAMETER                = 0.4572f;
static const float  ARC_LENGTH_PER_RELUCTOR_TOOTH =
    (float)(((float)M_PI * TIRE_DIAMETER) / (float)RELUCTOR_RING_TOOTH_COUNT);

static struct FreqOnlyPwmInput *left_wheel_speed_sensor, *right_wheel_speed_sensor;

void Io_WheelSpeedSensors_Init(
    TIM_HandleTypeDef *htim_left_wheel_speed_sensor,
    TIM_HandleTypeDef *htim_right_wheel_speed_sensor)
{
    assert(htim_left_wheel_speed_sensor != NULL);
    assert(htim_right_wheel_speed_sensor != NULL);

    left_wheel_speed_sensor = Io_SharedFreqOnlyPwmInput_Create(
        htim_left_wheel_speed_sensor, TIMx_FREQUENCY / TIM16_PRESCALER, TIM_CHANNEL_1, TIM16_AUTO_RELOAD_REG,
        HAL_TIM_ACTIVE_CHANNEL_1);

    right_wheel_speed_sensor = Io_SharedFreqOnlyPwmInput_Create(
        htim_right_wheel_speed_sensor, TIMx_FREQUENCY / TIM17_PRESCALER, TIM_CHANNEL_1, TIM17_AUTO_RELOAD_REG,
        HAL_TIM_ACTIVE_CHANNEL_1);
}

float Io_WheelSpeedSensors_GetLeftSpeedKph(void)
{
    return MPS_TO_KPH_CONVERSION_FACTOR * ARC_LENGTH_PER_RELUCTOR_TOOTH *
           Io_SharedFreqOnlyPwmInput_GetFrequency(left_wheel_speed_sensor);
}

float Io_WheelSpeedSensors_GetRightSpeedKph(void)
{
    return MPS_TO_KPH_CONVERSION_FACTOR * ARC_LENGTH_PER_RELUCTOR_TOOTH *
           Io_SharedFreqOnlyPwmInput_GetFrequency(right_wheel_speed_sensor);
}

void Io_WheelSpeedSensors_InputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(left_wheel_speed_sensor) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(left_wheel_speed_sensor))
    {
        Io_SharedFreqOnlyPwmInput_Tick(left_wheel_speed_sensor);
    }

    else if (
        htim == Io_SharedFreqOnlyPwmInput_GetTimerHandle(right_wheel_speed_sensor) &&
        htim->Channel == Io_SharedFreqOnlyPwmInput_GetTimerActiveChannel(right_wheel_speed_sensor))
    {
        Io_SharedFreqOnlyPwmInput_Tick(right_wheel_speed_sensor);
    }
}

void Io_WheelSpeedSensors_CheckIfRightSensorIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(right_wheel_speed_sensor);
}

void Io_WheelSpeedSensors_CheckIfLeftSensorIsActive(void)
{
    Io_SharedFreqOnlyPwmInput_CheckIfPwmIsActive(left_wheel_speed_sensor);
}
