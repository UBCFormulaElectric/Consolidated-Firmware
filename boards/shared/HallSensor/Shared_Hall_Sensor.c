#include "Shared_Hall_Sensor.h"

#define prvGetSettingsFromHandle(handle) (HallSensor_Settings_t *)(handle)
#define prvGetHallSensorHandle(handle) (HallSensor_t *)(handle)

extern TIM_HandleTypeDef htim2;

HallSensor_Handle Shared_Init_Hall_Sensor(HallSensor_Handle *_settings)
{
    shared_assert(_settings != NULL);

    HallSensor_Settings_t *settings = prvGetSettingsFromHandle(_settings);

    static HallSensor_t Hall_Sensor;
    Hall_Sensor.settings = settings;

    HAL_TIM_IC_Start_IT((settings->htim), settings->rising_edge_tim_channel);

    return (HallSensor_Handle *)&Hall_Sensor;
}

float32_t *Shared_Update_Freq_Hall_Sensor(HallSensor_Handle *_hall_sensor)
{
    HallSensor_t *hall_sensor = prvGetHallSensorHandle(_hall_sensor);

    shared_assert(hall_sensor != NULL);
    shared_assert(hall_sensor->settings != NULL);

    // Get measured frequency

    uint32_t  ic_rising_edge;
    uint32_t  getClkFreq;
    float32_t measuredFrequency;

    ic_rising_edge = HAL_TIM_ReadCapturedValue(
        (hall_sensor->settings->htim),
        hall_sensor->settings->rising_edge_tim_channel);

    getClkFreq        = HAL_RCC_GetPCLK2Freq();
    measuredFrequency = (float32_t)getClkFreq / ic_rising_edge;

    // Update measured frequency
    hall_sensor->frequency_ptr = &measuredFrequency;

    // Return pointer to the measured frequency value

    return hall_sensor->frequency_ptr;
}
