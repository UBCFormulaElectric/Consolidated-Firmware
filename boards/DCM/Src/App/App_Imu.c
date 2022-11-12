#include <assert.h>
#include <stdlib.h>

#include "App_Imu.h"
#include "Io_Imu.h"

typedef struct
{
    InRangeCheck acceleration_x_in_range_check;
    InRangeCheck acceleration_y_in_range_check;
    InRangeCheck acceleration_z_in_range_check;
} ImuData;

static ImuData imu;

void App_Imu_Init()
{
    imu.acceleration_x_in_range_check =
        *App_InRangeCheck_Create(App_Imu_GetAccelerationX, MIN_ACCELERATION_MS2, MAX_ACCELERATION_MS2);
    imu.acceleration_y_in_range_check =
        *App_InRangeCheck_Create(App_Imu_GetAccelerationY, MIN_ACCELERATION_MS2, MAX_ACCELERATION_MS2);
    imu.acceleration_z_in_range_check =
        *App_InRangeCheck_Create(App_Imu_GetAccelerationZ, MIN_ACCELERATION_MS2, MAX_ACCELERATION_MS2);
}
float App_Imu_GetAccelerationX()
{
    return Io_Imu_GetAccelerationX();
}

float App_Imu_GetAccelerationY()
{
    return Io_Imu_GetAccelerationY();
}

float App_Imu_GetAccelerationZ()
{
    return Io_Imu_GetAccelerationZ();
}

InRangeCheck *App_Imu_GetAccelerationXInRangeCheck()
{
    return &imu.acceleration_x_in_range_check;
}

InRangeCheck *App_Imu_GetAccelerationYInRangeCheck()
{
    return &imu.acceleration_y_in_range_check;
}

InRangeCheck *App_Imu_GetAccelerationZInRangeCheck()
{
    return &imu.acceleration_z_in_range_check;
}
