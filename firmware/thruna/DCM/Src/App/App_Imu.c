#include <assert.h>
#include <stdlib.h>

#include "App_InRangeCheck.h"
#include "App_Imu.h"

struct Imu
{
    struct InRangeCheck *acceleration_x_in_range_check;
    struct InRangeCheck *acceleration_y_in_range_check;
    struct InRangeCheck *acceleration_z_in_range_check;

    float (*get_acceleration_x)(void);
    float (*get_acceleration_y)(void);
    float (*get_acceleration_z)(void);
};

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

struct InRangeCheck *App_Imu_GetAccelerationXInRangeCheck(const struct Imu *const imu)
{
    return imu->acceleration_x_in_range_check;
}

struct InRangeCheck *App_Imu_GetAccelerationYInRangeCheck(const struct Imu *const imu)
{
    return imu->acceleration_y_in_range_check;
}

struct InRangeCheck *App_Imu_GetAccelerationZInRangeCheck(const struct Imu *const imu)
{
    return imu->acceleration_z_in_range_check;
}

struct Imu *App_Imu_Create(
    float (*get_acceleration_x)(void),
    float (*get_acceleration_y)(void),
    float (*get_acceleration_z)(void),
    float min_acceleration,
    float max_acceleration)
{
    struct Imu *imu = malloc(sizeof(struct Imu));
    assert(imu != NULL);

    imu->acceleration_x_in_range_check =
        App_InRangeCheck_Create(get_acceleration_x, min_acceleration, max_acceleration);
    imu->acceleration_y_in_range_check =
        App_InRangeCheck_Create(get_acceleration_y, min_acceleration, max_acceleration);
    imu->acceleration_z_in_range_check =
        App_InRangeCheck_Create(get_acceleration_z, min_acceleration, max_acceleration);

    imu->get_acceleration_x = get_acceleration_x;
    imu->get_acceleration_y = get_acceleration_y;
    imu->get_acceleration_z = get_acceleration_z;

    return imu;
}

void App_Imu_Destroy(struct Imu *imu)
{
    free(imu->acceleration_x_in_range_check);
    free(imu->acceleration_y_in_range_check);
    free(imu->acceleration_z_in_range_check);
    free(imu);
}
