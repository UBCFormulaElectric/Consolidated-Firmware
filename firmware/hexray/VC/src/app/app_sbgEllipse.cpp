
#include "app_sbgEllipse.h"
extern "C"
{
#include <assert.h>
#include <math.h>
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
#include "io_log.h"
#include "io_sbgEllipse.h"
}

namespace app::sbgEllipse
{
static consteval float       vehicle_velocity;
static consteval VcEkfStatus ekf_solution_mode;

void app_sbgEllipse_broadcast()
{
    /* Enable these back when you turn this on in the SBG, otherwise it's still sending
       CAN messages because another message in the signal is being used */

    // Status msg
    app_canTx_VC_EllipseGeneralStatusBitmask_set(io_sbgEllipse_getGeneralStatus());
    app_canTx_VC_EllipseComStatusBitmask_set(io_sbgEllipse_getComStatus());
    app_canTx_VC_EllipseQueueOverflowCount_set(io_sbgEllipse_getOverflowCount());

    // Time msg
    constexpr uint32_t timestamp_us = io_sbgEllipse_getTimestampUs();
    app_canTx_VC_EllipseTimestamp_set(timestamp_us);

    // VelocityData velocity_calculated;

    // calculating velocity data based on wheel speed
    // app_sbgEllipse_calculateVelocity(&velocity_calculated);

    // EKF
    // float ekf_vel_N = io_sbgEllipse_getEkfNavVelocityData()->north;
    // float ekf_vel_E = io_sbgEllipse_getEkfNavVelocityData()->east;
    // float ekf_vel_D = io_sbgEllipse_getEkfNavVelocityData()->down;

    // const float ekf_vel_N_accuracy = io_sbgEllipse_getEkfNavVelocityData()->north_std_dev;
    // const float ekf_vel_E_accuracy = io_sbgEllipse_getEkfNavVelocityData()->east_std_dev;
    // const float ekf_vel_D_accuracy = io_sbgEllipse_getEkfNavVelocityData()->down_std_dev;
    const float ekf_vel_N_accuracy = 0;
    const float ekf_vel_E_accuracy = 0;
    const float ekf_vel_D_accuracy = 0;

    app_canTx_VC_VelocityNorth_set(ekf_vel_N);
    app_canTx_VC_VelocityEast_set(ekf_vel_E);
    app_canTx_VC_VelocityDown_set(ekf_vel_D);

    app_canTx_VC_VelocityNorthAccuracy_set(ekf_vel_N_accuracy);
    app_canTx_VC_VelocityEastAccuracy_set(ekf_vel_E_accuracy);
    app_canTx_VC_VelocityDownAccuracy_set(ekf_vel_D_accuracy);

    vehicle_velocity = sqrtf(SQUARE(ekf_vel_N) + SQUARE(ekf_vel_E) + SQUARE(ekf_vel_D));
    // const float vehicle_velocity_calculated = MPS_TO_KMH(velocity_calculated.north);

    ekf_solution_mode = (VcEkfStatus)io_sbgEllipse_getEkfSolutionMode();

    if (ekf_solution_mode < NUM_VC_EKF_STATUS_CHOICES)
    {
        // app_canTx_VC_EkfSolutionMode_set(ekf_solution_mode);
    }

    // Euler angles msg
    // Need to change these to cpp in io
    // const float euler_roll  = io_sbgEllipse_getEkfEulerAngles()->roll;
    // const float euler_pitch = io_sbgEllipse_getEkfEulerAngles()->pitch;
    // const float euler_yaw   = io_sbgEllipse_getEkfEulerAngles()->yaw;
    const float euler_roll  = 0;
    const float euler_pitch = 0;
    const float euler_yaw   = 0;

    app_canTx_VC_EulerAnglesRoll_set(euler_roll);
    app_canTx_VC_EulerAnglesPitch_set(euler_pitch);
    app_canTx_VC_EulerAnglesYaw_set(euler_yaw);
}

float app_sbgEllipse_getVehicleVelocity(void)
{
    return vehicle_velocity;
}

VcEkfStatus app_sbgEllipse_getEkfSolutionMode(void)
{
    return ekf_solution_mode;
}
} // namespace app::sbgEllipse
