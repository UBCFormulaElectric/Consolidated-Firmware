#include <assert.h>
#include <stdlib.h>

#include "App_Imu.h"

struct Imu
{
    ExitCode (*update_sensor_data)(void);
    float (*get_acceleration_x)(void);
    float (*get_acceleration_y)(void);
    float (*get_acceleration_z)(void);
};

ExitCode App_Imu_UpdateSensorData(const struct Imu *const imu)
{
    return imu->update_sensor_data();
}

float App_Imu_GetAccelerationX(const struct Imu *const imu)
{
    return imu->get_acceleration_x();
}

float App_Imu_GetAccelerationY(const struct Imu *const imu)
{
    return imu->get_acceleration_y();
}

float App_Imu_GetAccelerationZ(const struct Imu *const imu)
{
    return imu->get_acceleration_z();
}

struct Imu *App_Imu_Create(
    ExitCode (*update_sensor_data)(void),
    float (*get_acceleration_x)(void),
    float (*get_acceleration_y)(void),
    float (*get_acceleration_z)(void))
{
    struct Imu *imu = malloc(sizeof(struct Imu));
    assert(imu != NULL);

    imu->update_sensor_data = update_sensor_data;
    imu->get_acceleration_x = get_acceleration_x;
    imu->get_acceleration_y = get_acceleration_y;
    imu->get_acceleration_z = get_acceleration_z;

    return imu;
}

void App_Imu_Destroy(struct Imu *imu)
{
    free(imu);
}
