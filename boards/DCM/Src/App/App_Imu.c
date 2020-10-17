#include <assert.h>
#include <stdlib.h>

#include "App_Imu.h"

struct Imu
{
    bool (*update_data)(void);
    bool (*is_data_valid)(void);
    struct ImuData *(*get_imu_data)(void);
};

bool App_Imu_UpdateData(const struct Imu *imu)
{
    return imu->update_data();
}

float App_Imu_GetAccelerationX(const struct Imu *imu)
{
    return imu->get_imu_data()->accel_x;
}

float App_Imu_GetAccelerationY(const struct Imu *imu)
{
    return imu->get_imu_data()->accel_y;
}

float App_Imu_GetAccelerationZ(const struct Imu *imu)
{
    return imu->get_imu_data()->accel_z;
}

struct Imu *App_Imu_Create(
    bool (*update_data)(void),
    struct ImuData *(*get_imu_data)(void))
{
    struct Imu *imu = malloc(sizeof(struct Imu));
    assert(imu != NULL);

    imu->update_data  = update_data;
    imu->get_imu_data = get_imu_data;

    return imu;
}
