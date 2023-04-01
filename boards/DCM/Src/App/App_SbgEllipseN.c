#include <assert.h>
#include "App_SbgEllipseN.h"
#include "App_CanTx.h"

typedef struct
{
    void (*handle_logs)(void);
    float (*get_sensor_output)(SbgSensorOutput);
} SbgEllipseNSensor;

static SbgEllipseNSensor sensor;

void App_SbgEllipseN_Init(
    void (*handle_logs)(void),
    uint32_t (*get_timestamp)(void),
    float (*get_sensor_output)(SbgSensorOutput))
{
    sensor.handle_logs       = handle_logs;
    sensor.get_sensor_output = get_sensor_output;
}

void App_SbgEllipseN_HandleLogs()
{
    sensor.handle_logs();
}

void App_SbgEllipseN_Broadcast()
{
    const int euler_roll  = (int)sensor.get_sensor_output(SBG_OUTPUT_ROLL);
    const int euler_pitch = (int)sensor.get_sensor_output(SBG_OUTPUT_PITCH);
    const int euler_yaw   = (int)sensor.get_sensor_output(SBG_OUTPUT_YAW);

    App_CanTx_DCM_SbgEllipseSensor_Roll_Set(euler_roll);
    App_CanTx_DCM_SbgEllipseSensor_Pitch_Set(euler_pitch);
    App_CanTx_DCM_SbgEllipseSensor_Yaw_Set(euler_yaw);
}

float App_SbgEllipseN_GetSensorOutput(SbgSensorOutput output)
{
    assert(output < NUM_SBG_OUTPUTS);
    return sensor.get_sensor_output(output);
}
