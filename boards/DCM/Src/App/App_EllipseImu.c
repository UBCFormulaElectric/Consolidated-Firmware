#include <assert.h>
#include "App_EllipseImu.h"
#include "App_CanTx.h"

typedef struct
{
    void (*handle_logs)(void);
    uint16_t (*get_general_status)(void);
    uint32_t (*get_com_status)(void);
    uint32_t (*get_timestamp)(void);
    float (*get_sensor_output)(SbgSensorOutput);
} EllipseImuSensor;

static EllipseImuSensor sensor;

void App_EllipseImu_Init(
    void (*handle_logs)(void),
    uint32_t (*get_timestamp)(void),
    uint16_t (*get_general_status)(void),
    uint32_t (*get_com_status)(void),
    float (*get_sensor_output)(SbgSensorOutput))
{
    sensor.handle_logs        = handle_logs;
    sensor.get_timestamp      = get_timestamp;
    sensor.get_general_status = get_general_status;
    sensor.get_com_status     = get_com_status;
    sensor.get_sensor_output  = get_sensor_output;
}

void App_EllipseImu_HandleLogs()
{
    sensor.handle_logs();
}

void App_EllipseImu_Broadcast()
{
    // Status msg
    const uint16_t general_status = sensor.get_general_status();
    const uint32_t com_status     = sensor.get_com_status();

    App_CanTx_DCM_EllipseImuStatus_GeneralStatusBitmask_Set(general_status);
    App_CanTx_DCM_EllipseImuStatus_ComStatusBitmask_Set(com_status);

    // Time msg
    const uint32_t timestamp_us = App_EllipseImu_GetTimestampUs();
    App_CanTx_DCM_EllipseImuTime_Timestamp_Set(timestamp_us);

    // Acceleration msg
    const float forward_accel  = sensor.get_sensor_output(SBG_OUTPUT_ACCELERATION_X);
    const float lateral_accel  = sensor.get_sensor_output(SBG_OUTPUT_ACCELERATION_Y);
    const float vertical_accel = sensor.get_sensor_output(SBG_OUTPUT_ACCELERATION_Z);

    App_CanTx_DCM_EllipseImuAcceleration_Forward_Set(forward_accel);
    App_CanTx_DCM_EllipseImuAcceleration_Lateral_Set(lateral_accel);
    App_CanTx_DCM_EllipseImuAcceleration_Vertical_Set(vertical_accel);

    // Angular velocity msg
    const float ang_vel_roll  = sensor.get_sensor_output(SBG_OUTPUT_ANGULAR_VELOCITY_ROLL);
    const float ang_vel_pitch = sensor.get_sensor_output(SBG_OUTPUT_ANGULAR_VELOCITY_PITCH);
    const float ang_vel_yaw   = sensor.get_sensor_output(SBG_OUTPUT_ANGULAR_VELOCITY_YAW);

    App_CanTx_DCM_EllipseImuAngularVelocity_Roll_Set((int)ang_vel_roll);
    App_CanTx_DCM_EllipseImuAngularVelocity_Pitch_Set((int)ang_vel_pitch);
    App_CanTx_DCM_EllipseImuAngularVelocity_Yaw_Set((int)ang_vel_yaw);

    // Euler angles msg
    const float euler_roll  = sensor.get_sensor_output(SBG_OUTPUT_EULER_ROLL);
    const float euler_pitch = sensor.get_sensor_output(SBG_OUTPUT_EULER_PITCH);
    const float euler_yaw   = sensor.get_sensor_output(SBG_OUTPUT_EULER_YAW);

    App_CanTx_DCM_EllipseImuEulerAngles_Roll_Set(euler_roll);
    App_CanTx_DCM_EllipseImuEulerAngles_Pitch_Set(euler_pitch);
    App_CanTx_DCM_EllipseImuEulerAngles_Yaw_Set(euler_yaw);
}

uint32_t App_EllipseImu_GetTimestampUs()
{
    return sensor.get_timestamp();
}

float App_EllipseImu_GetSensorOutput(SbgSensorOutput output)
{
    assert(output < NUM_SBG_OUTPUTS);
    return sensor.get_sensor_output(output);
}
