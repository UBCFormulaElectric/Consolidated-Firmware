#include "flowMeter_driver.h"
#include "Shared_Hall_Sensor.h"

#define FLOWMETER_TIMER_CHANNEL TIM_CHANNEL_3
#define FLOWMETER_TIMER_FREQUENCY 36000000

// Currently configured to TIM2; Channel 3
extern TIM_HandleTypeDef htim2;

typedef struct
{
    HallSensor_Handle _flow_meter;
    float32_t *       _flow_rate;

} FlowMeter_t;

#define prvGetFlowMeterFromHandle(handle) (FlowMeter_t *)(handle)

// Initialize settings for the Hall Effect Sensor
static HallSensor_Settings_Handle _Init_FlowMeter_Settings(void)
{
    static HallSensor_Settings_t hall_sensor_settings;

    hall_sensor_settings.htim                    = &htim2;
    hall_sensor_settings.rising_edge_tim_channel = FLOWMETER_TIMER_CHANNEL;
    hall_sensor_settings.timer_frequency_hz      = FLOWMETER_TIMER_FREQUENCY;

    return (HallSensor_Settings_Handle)&hall_sensor_settings;
}

static HallSensor_Handle _Init_FlowMeter_Handle(void)
{
    // Init Flow Meter Settings
    HallSensor_Settings_Handle FlowMeter_Settings = _Init_FlowMeter_Settings();

    // Give Flow Meter Settings to Hall Sensor Struct
    return Shared_Init_Hall_Sensor(FlowMeter_Settings);
}
int flow_rate = 0;

static float32_t *_Get_Flow_Rate_Ptr(HallSensor_Handle _Hall_Sensor)
{
    // Check that settings have been configured
    shared_assert(_Hall_Sensor != NULL);

    // Collect measured frequency pointer and store in a tmp pointer
    float32_t *data_ptr = Shared_Update_Freq_Hall_Sensor(_Hall_Sensor);

    // todo: delete
    float32_t data = *data_ptr;
    UNUSED(data);
    // Dereference pointer and convert frequency into a flow rate and return
    // pointer
    static float32_t flow_rate;
    flow_rate = (float32_t) *(data_ptr) / 7.5;

    // Use temp pointer to point to calculated flow rate and return
    data_ptr = &flow_rate;

    return data_ptr;
}

FlowMeter_Handle _Init_FlowMeter(void)
{
    static FlowMeter_t FlowMeter;

    FlowMeter._flow_meter = _Init_FlowMeter_Handle();
    FlowMeter._flow_rate  = _Get_Flow_Rate_Ptr(FlowMeter._flow_meter);

    return (FlowMeter_Handle)&FlowMeter;
}

void _Update_Flow_Rate(FlowMeter_Handle _Flow_Meter)
{
    shared_assert(_Flow_Meter != NULL);

    FlowMeter_t *Flow_Meter = prvGetFlowMeterFromHandle(_Flow_Meter);

    Flow_Meter->_flow_rate = _Get_Flow_Rate_Ptr(Flow_Meter->_flow_meter);
}

float32_t _get_flow_rate(FlowMeter_Handle _flow_meter)
{
    shared_assert(_flow_meter != NULL);
    FlowMeter_t *Flow_Meter = prvGetFlowMeterFromHandle(_flow_meter);

    float32_t temp = *(Flow_Meter->_flow_rate);

    return temp;
}

void _show_struct_content(FlowMeter_Handle _flow_meter)
{
    shared_assert(_flow_meter != NULL);

    FlowMeter_t *Flow_Meter = prvGetFlowMeterFromHandle(_flow_meter);

    uint32_t slatt;
    slatt = 0;

    UNUSED(slatt);

    UNUSED(Flow_Meter);
}
