// yaw rate should be an input here
// slip should be calculated and used as a constriant here 
// load transfer should be applied here 
// output torques to be used by power limiting

#include "app_torque_allocator.h"


//--------------------- Constants and Datatypes ------------------------------------------------//

//----------------------- Static Function Prototypes -------------------------------------------//
static SensorStatus check_sensors(void);


//----------------------- Global Functions -----------------------------------------------------//

TorqueAllocationOutputs app_torque_allocator(void)
{

    const bool regen_enabled = app_canTx_VC_RegenEnabled_get(); // IMPORTANT: this must be calculated in state machine

    double apps_pedal_decimal = (double)app_canRx_FSM_PappsMappedPedalPercentage_get() * 0.01;
    if (regen_enabled)
    {
        apps_pedal_decimal = app_regen_pedalRemapping(apps_pedal_decimal);
    }
    app_canTx_VC_RegenMappedPedalPercentage_set((float)apps_pedal_decimal * 100.0f);

    if (apps_pedal_decimal < 0.0)
    {
        // TODO incorperate negative torques into the algorithm
        return app_regen_run(apps_pedal_decimal);
    }

    if (SWITCH_ON == app_canRx_CRIT_VanillaOverrideSwitch_get())
    {
        // TODO vanilla handling
    }

    SensorStatus sensor_states = check_sensors();
    const double wheel_angle = (double)app_canRx_FSM_SteeringAngle_get() * APPROX_STEERING_TO_WHEEL_ANGLE;
    const double power_limit = app_powerLimiting_computeMaxPower(false);

    TorqueAllocationOutputs torque_to_inverters; 

    switch(app_canRx_CRIT_DriveMode_get())
    {
        
        case DRIVE_MODE_POWER_AND_ACTIVE:

        case DRIVE_MODE_TV:

        case DRIVE_MODE_POWER:
        default:
            const static double F = TRACK_WIDTH_m / (WHEEL_DIAMETER_IN / 2.0 * 2.4) * GEAR_RATIO;

            const double load_transfer_const  = 1.0,
                         total_torque_request = apps_pedal_decimal * MAX_TORQUE_REQUEST_NM * 4, front_yaw_moment = 0.0,
                         rear_yaw_moment = 0.0;

            // Commented out can messages, there solely for logging purposes
            app_canTx_VC_RearYawMoment_set((float)rear_yaw_moment);
            app_canTx_VC_FrontYawMoment_set((float)front_yaw_moment);

            torque_to_inverters.torque_fl = total_torque_request * load_transfer_const / (2 * (load_transfer_const + 1)) -
                            front_yaw_moment / (2 * F);
            torque_to_inverters.torque_fr = torque_to_inverters.torque_fl + front_yaw_moment / F;
            torque_to_inverters.torque_rl =
                total_torque_request / 2 - total_torque_request * load_transfer_const / (2 * (load_transfer_const + 1));
            torque_to_inverters.torque_rr = torque_to_inverters.torque_rl + rear_yaw_moment / F;

            const double total_power    = app_totalPower(&torque_to_inverters);
            const double power_limit_kw = app_powerLimiting_computeMaxPower(false);
            app_canTx_VC_RequestedPower_set((float)total_power);
            app_canTx_VC_TotalAllocatedPower_set((float)total_power);
            app_powerLimiting_torqueReduction(false, &torque_to_inverters, app_totalPower(&torque_to_inverters), power_limit_kw, 0.0);

            app_canTx_VC_VcDriveMode_set(DRIVE_MODE_POWER);
            LOG_INFO("DriveHandling: PowerLimit Mode Active");
            break;




    }

}


//----------------------- Static Function Definition -------------------------------------------//
static SensorStatus check_sensors(void)
{
    SensorStatus sensor_status;
    sensor_status.gpsOk =
        !(app_canAlerts_VC_Info_SbgInitFailed_get() || app_sbgEllipse_getEkfSolutionMode() != POSITION);
    sensor_status.imuOk = !app_canAlerts_VC_Info_ImuInitFailed_get();
    sensor_status.steeringOk =
        !(app_canRx_FSM_Info_SteeringAngleOCSC_get() || app_canRx_FSM_Info_SteeringAngleOutOfRange_get());
    sensor_status.useTV = sensor_status.gpsOk && sensor_status.imuOk && sensor_status.steeringOk;

    if (!sensor_status.gpsOk)
        LOG_WARN("Sbg Ellipse not ok.");
    if (!sensor_status.imuOk)
        LOG_WARN("Imu not ok.");
    if (!sensor_status.steeringOk)
        LOG_WARN("Steering not ok");
    return sensor_status;
}


