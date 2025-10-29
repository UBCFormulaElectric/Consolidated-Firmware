#include "app_stateEstimation.h"

#include "app_canTx.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_imu.h"

#include "io_imu_config.h"

VD_State state;

static double calculateLoadTransfer(const double lat_accel)
{
    return CAR_MASS_AT_CG_KG * lat_accel * CG_HEIGHT_FROM_GROUND_M / (2.0 * (TRACK_WIDTH_m));
}

void app_stateEstimation_estimate(void)
{
    const ImuData *imu_data = app_get_imu_struct();

    const double fzf_mass = (REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0,
                 fzr_mass = (REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(imu_data->long_accel)) / 4.0;

    const double load_transfer = calculateLoadTransfer(imu_data->lat_accel);
    state.Fz_fl                = fzf_mass - load_transfer;
    state.Fz_fr                = fzf_mass + load_transfer;
    state.Fz_rl                = fzr_mass - load_transfer;
    state.Fz_rr                = fzr_mass + load_transfer;

    app_canTx_VC_FrontLeftWheelVerticalForce_set((uint32_t)state.Fz_fl);
    app_canTx_VC_FrontRightWheelVerticalForce_set((uint32_t)state.Fz_fr);
    app_canTx_VC_RearLeftWheelVerticalForce_set((uint32_t)state.Fz_rl);
    app_canTx_VC_RearRightWheelVerticalForce_set((uint32_t)state.Fz_rr);
}
