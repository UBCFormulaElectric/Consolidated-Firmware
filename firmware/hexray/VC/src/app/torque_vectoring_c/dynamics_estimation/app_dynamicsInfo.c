#include "app_dynamicsInfo.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_vd_constants.h"


/** Responsibilities
 * slip ratio estimation
 * slip angle estimation
 * drag force estimation
 * down force estimation
 * normal force estimation
 * 
 */
/************************* Private Datatypes *****************************/
static VD_dynamicsEst_inputs inputs; 
static VD_dynamicsEst_outputs outputs;

/************************* Private Function Prototypes *****************************/
static void app_dynamicsInfo_getInputs(void);
static void app_dynamicsInfo_setOutputs(void);
static inline void app_dynamicsInfo_calcNormalForce(void);
static inline void app_dynamicsInfo_calcLoadTransfer(void);
static inline void app_dynamicsInfo_calcSlipRatio(void);
static inline void app_dynamicsInfo_calcSlipAngle(void); 
static inline void app_dynamicsInfo_calcResistiveForce(void); 

/************************* Global Function Definition *****************************/

VD_dynamicsEst_outputs app_dynamicsInfo_estimate(void)
{
    app_dynamicsInfo_getInputs();

    app_dynamicsInfo_calcLoadTransfer();
    app_dynamicsInfo_calcEffectiveRadius();
    app_dynamicsInfo_calcResistiveForce();
    app_dynamicsInfo_calcNormalForce();
    app_dynamicsInfo_calcSlipAngle();
    app_dynamicsInfo_calcSlipRatio();

    app_dynamicsInfo_setOutputs();

    return outputs;
}

/************************* Private Function Definition *****************************/

static void app_dynamicsInfo_getInputs(void)
{
    inputs->imu_data = app_get_imu_struct();
    inputs.wheel_speed_fl  = MOTOR_RPM_TO_KMH(fabsf((float)app_canRx_INVFR_ActualVelocity_get()));
    inputs.wheel_speed_fr  = MOTOR_RPM_TO_KMH(fabsf((float)app_canRx_INVFL_ActualVelocity_get()));
    inputs.wheel_speed_rl  = MOTOR_RPM_TO_KMH(fabsf((float)app_canRx_INVRR_ActualVelocity_get()));
    inputs.wheel_speed_rr  = MOTOR_RPM_TO_KMH(fabsf((float)app_canRx_INVRL_ActualVelocity_get()));
}

static void app_dynamicsInfo_setOutputs()
{
    // 
}

/************************* Helper Function Definition *****************************/
static inline void app_dynamicsInfo_calcNormalForce(void)
{

   outputs.tireModel.Fz_fl  = FRONT_CONST_LOAD - outputs.lat_loadTransfer - outputs.long_loadTransfer;
   outputs.tireModel.Fz_fr  = FRONT_CONST_LOAD + outputs.lat_loadTransfer - outputs.long_loadTransfer;
   outputs.tireModel.Fz_rl  = REAR_CONST_LOAD - outputs.lat_loadTransfer + outputs.long_loadTransfer;
   outputs.tireModel.Fz_rr  = REAR_CONST_LOAD + outputs.lat_loadTransfer + outputs.long_loadTransfer;
}

static inline void app_dynamicsInfo_calcLoadTransfer(void)
{
    outputs.lat_loadTransfer = LAT_LOAD_TRANSFER(inputs.imu_data->lat_accel) / 4.0;
    outputs.long_loadTransfer = LONG_LOAD_TRANSFER(inputs.imu_data->long_accel) / 4.0;
}

static inline void app_dynamicsInfo_calcSlipRatio(void)
{
   outputs.tireModel.SR_fl = SLIP_RATIO(inputs.wheel_speed_fl,inputs.vehicle_speed);
   outputs.tireModel.SR_fr = SLIP_RATIO(inputs.wheel_speed_fr,inputs.vehicle_speed);
   outputs.tireModel.SR_fl = SLIP_RATIO(inputs.wheel_speed_rl,inputs.vehicle_speed);
   outputs.tireModel.SR_fl = SLIP_RATIO(inputs.wheel_speed_rr,inputs.vehicle_speed);
}

// TODO: figure out how to calculate slip angle
static inline void app_dynamicsInfo_calcSlipAngle(void)
{
    outputs.tireModel.SA_fl = 0.0;
    outputs.tireModel.SA_fr = 0.0;
    outputs.tireModel.SA_rl = 0.0;
    outputs.tireModel.SA_rr = 0.0;
}

// TODO: figure out how to calculate and use drag and down force 
static inline void app_dynamicsInfo_calcResistiveForce(void)
{
    outputs.Fx_drag = 0.0;
    outputs.Fz_down_force = 0.0;
}

// TODO: figure out how to find effective radius (to keep things consistent lets use empirical units so meters)
static inline void app_dynamicsInfo_calcEffectiveRadius(void)
{
    outputs.tireModel.eff_rad_fl = IN_TO_M * WHEEL_DIAMETER_IN / 2;
    outputs.tireModel.eff_rad_fr = IN_TO_M * WHEEL_DIAMETER_IN / 2; 
    outputs.tireModel.eff_rad_rl = IN_TO_M * WHEEL_DIAMETER_IN / 2; 
    outputs.tireModel.eff_rad_rr = IN_TO_M * WHEEL_DIAMETER_IN / 2; 
}



