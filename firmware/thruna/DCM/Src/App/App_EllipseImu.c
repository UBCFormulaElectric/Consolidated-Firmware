#include <assert.h>
#include "App_EllipseImu.h"
#include "App_CanTx.h"

typedef struct
{
    void (*handle_logs)(void);
    uint16_t (*get_general_status)(void);
    uint32_t (*get_com_status)(void);
    uint32_t (*get_timestamp)(void);
    float (*get_sensor_output)(EllipseImuOutput);
} EllipseImuSensor;

static EllipseImuSensor sensor;

void App_EllipseImu_Init(
    void (*handle_logs)(void),
    uint32_t (*get_timestamp)(void),
    uint16_t (*get_general_status)(void),
    uint32_t (*get_com_status)(void),
    float (*get_sensor_output)(EllipseImuOutput))
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

    App_CanTx_DCM_EllipseGeneralStatusBitmask_Set(general_status);
    App_CanTx_DCM_EllipseComStatusBitmask_Set(com_status);

    // Time msg
    const uint32_t timestamp_us = App_EllipseImu_GetTimestampUs();
    App_CanTx_DCM_EllipseTimestamp_Set(timestamp_us);

    // Acceleration msg
    const float forward_accel  = sensor.get_sensor_output(ELLIPSE_OUTPUT_ACCELERATION_X);
    const float lateral_accel  = sensor.get_sensor_output(ELLIPSE_OUTPUT_ACCELERATION_Y);
    const float vertical_accel = sensor.get_sensor_output(ELLIPSE_OUTPUT_ACCELERATION_Z);

    App_CanTx_DCM_AccelerationForward_Set(forward_accel);
    App_CanTx_DCM_AccelerationLateral_Set(lateral_accel);
    App_CanTx_DCM_AccelerationVertical_Set(vertical_accel);

    // Angular velocity msg
    const float ang_vel_roll  = sensor.get_sensor_output(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_ROLL);
    const float ang_vel_pitch = sensor.get_sensor_output(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_PITCH);
    const float ang_vel_yaw   = sensor.get_sensor_output(ELLIPSE_OUTPUT_ANGULAR_VELOCITY_YAW);

    App_CanTx_DCM_AngularVelocityRoll_Set((int)ang_vel_roll);
    App_CanTx_DCM_AngularVelocityPitch_Set((int)ang_vel_pitch);
    App_CanTx_DCM_AngularVelocityYaw_Set((int)ang_vel_yaw);

    // Euler angles msg
    const float euler_roll  = sensor.get_sensor_output(ELLIPSE_OUTPUT_EULER_ROLL);
    const float euler_pitch = sensor.get_sensor_output(ELLIPSE_OUTPUT_EULER_PITCH);
    const float euler_yaw   = sensor.get_sensor_output(ELLIPSE_OUTPUT_EULER_YAW);

    App_CanTx_DCM_EulerAnglesRoll_Set(euler_roll);
    App_CanTx_DCM_EulerAnglesPitch_Set(euler_pitch);
    App_CanTx_DCM_EulerAnglesYaw_Set(euler_yaw);

    // Gps position msg
    const float position_status = sensor.get_sensor_output(GPS_POS_STATUS);
    const float latitude        = sensor.get_sensor_output(GPS_LAT);
    const float latitude_acc    = sensor.get_sensor_output(GPS_LAT_ACC);
    const float longitude       = sensor.get_sensor_output(GPS_LONG);
    const float longitude_acc   = sensor.get_sensor_output(GPS_LONG_ACC);
    const float altitude        = sensor.get_sensor_output(GPS_ALT);
    const float altitude_acc    = sensor.get_sensor_output(GPS_ALT_ACC);

    App_CanTx_DCM_PositionStatus_Set(position_status);
    App_CanTx_DCM_Latitude_Set(latitude);
    App_CanTx_DCM_LatitudeAccuracy_Set(latitude_acc);
    App_CanTx_DCM_Longtitude_Set(longitude);
    App_CanTx_DCM_LongtitudeAccuracy_Set(longitude_acc);
    App_CanTx_DCM_Altitude_Set(altitude);
    App_CanTx_DCM_AltitudeAccuracy_Set(altitude_acc);

    // Gps velocity msg
    const float velocity_status    = sensor.get_sensor_output(GPS_VEL_STATUS);
    const float velocity_north     = sensor.get_sensor_output(GPS_VEL_N);
    const float velocity_north_acc = sensor.get_sensor_output(GPS_VEL_N_ACC);
    const float velocity_east      = sensor.get_sensor_output(GPS_VEL_E);
    const float velocity_east_acc  = sensor.get_sensor_output(GPS_VEL_E_ACC);
    const float velocity_down      = sensor.get_sensor_output(GPS_VEL_D);
    const float velocity_down_acc  = sensor.get_sensor_output(GPS_VEL_D_ACC);

    App_CanTx_DCM_VelocityStatus_Set(velocity_status);
    App_CanTx_DCM_VelocityNorth_Set(velocity_north);
    App_CanTx_DCM_VelocityNorthAccuracy_Set(velocity_north_acc);
    App_CanTx_DCM_VelocityEast_Set(velocity_east);
    App_CanTx_DCM_VelocityEastAccuracy_Set(velocity_east_acc);
    App_CanTx_DCM_VelocityDown_Set(velocity_down);
    App_CanTx_DCM_VelocityDownAccuracy_Set(velocity_down_acc);
}

uint32_t App_EllipseImu_GetTimestampUs()
{
    return sensor.get_timestamp();
}

float App_EllipseImu_GetSensorOutput(EllipseImuOutput output)
{
    assert(output < NUM_SBG_OUTPUTS);
    return sensor.get_sensor_output(output);
}
