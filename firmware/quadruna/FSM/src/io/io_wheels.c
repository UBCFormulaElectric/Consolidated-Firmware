#include "io_wheels.h"
#include <assert.h>
#include <math.h>
#include "main.h"
#include "app_units.h"
#include "hw_pwmInputFreqOnly.h"

// Note: Unit for length is measured in metres unless specified
static const float  MPS_TO_KPH_CONVERSION_FACTOR = 3.6f;
static const size_t RELUCTOR_RING_TOOTH_COUNT    = 48;
static const float  ARC_LENGTH_PER_RELUCTOR_TOOTH =
    (float)(((float)M_PI * WHEEL_DIAMETER_IN * IN_TO_M) / (float)RELUCTOR_RING_TOOTH_COUNT);

static PwmInputFreqOnly left_wheel_speed_sensor, right_wheel_speed_sensor;

void io_wheels_init(TIM_HandleTypeDef *htim_left_wheel_speed_sensor, TIM_HandleTypeDef *htim_right_wheel_speed_sensor)
{
    assert(htim_left_wheel_speed_sensor != NULL);
    assert(htim_right_wheel_speed_sensor != NULL);

    hw_pwmInputFreqOnly_init(
        &left_wheel_speed_sensor, htim_left_wheel_speed_sensor, TIMx_FREQUENCY / TIM12_PRESCALER, TIM_CHANNEL_2,
        TIM12_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_2);

    hw_pwmInputFreqOnly_init(
        &right_wheel_speed_sensor, htim_right_wheel_speed_sensor, TIMx_FREQUENCY / TIM12_PRESCALER, TIM_CHANNEL_1,
        TIM12_AUTO_RELOAD_REG, HAL_TIM_ACTIVE_CHANNEL_1);
}

void io_wheels_inputCaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == hw_pwmInputFreqOnly_getTimerHandle(&left_wheel_speed_sensor) &&
        htim->Channel == hw_pwmInputFreqOnly_getTimerActiveChannel(&left_wheel_speed_sensor))
    {
        hw_pwmInputFreqOnly_tick(&left_wheel_speed_sensor);
    }

    else if (
        htim == hw_pwmInputFreqOnly_getTimerHandle(&right_wheel_speed_sensor) &&
        htim->Channel == hw_pwmInputFreqOnly_getTimerActiveChannel(&right_wheel_speed_sensor))
    {
        hw_pwmInputFreqOnly_tick(&right_wheel_speed_sensor);
    }
}

float io_wheels_getLeftSpeedKph(void)
{
    return MPS_TO_KPH_CONVERSION_FACTOR * ARC_LENGTH_PER_RELUCTOR_TOOTH *
           hw_pwmInputFreqOnly_getFrequency(&left_wheel_speed_sensor);
}

float io_wheels_getRightSpeedKph(void)
{
    return MPS_TO_KPH_CONVERSION_FACTOR * ARC_LENGTH_PER_RELUCTOR_TOOTH *
           hw_pwmInputFreqOnly_getFrequency(&right_wheel_speed_sensor);
}

void io_wheels_checkIfLeftSensorActive(void)
{
    hw_pwmInputFreqOnly_checkIfPwmIsActive(&left_wheel_speed_sensor);
}

void io_wheels_checkIfRightSensorActive(void)
{
    hw_pwmInputFreqOnly_checkIfPwmIsActive(&right_wheel_speed_sensor);
}
