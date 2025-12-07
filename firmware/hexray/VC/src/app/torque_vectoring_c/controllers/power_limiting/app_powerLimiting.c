#include "app_powerLimiting.h"
#include "app_vd_constants.h"
#include "app_powerLimiting_datatypes.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_vd_datatypes.h"

/************************* Macros *****************************/
#define TOTAL_WHEEL_RPM(fl_rpm, fr_rpm, rl_rpm, rr_rpm) ((double)((fl_rpm) + (fr_rpm) + (rl_rpm) + (rr_rpm)))

/************************* Private Function Prototypes *****************************/
static void app_powerLimiting_getInputs(VD_TorqueToInv torques, VD_WheelRpms wheel_rpms); 
static double app_powerLimiting_calcTorqueReduction(powerLimiting_inputs in, powerLimiting_outputs out);
static void app_powerLimiting_broadCast(powerLimiting_outputs out)
static inline double app_powerLimiting_calcMaxPower(bool isRegen_on);
static inline double app_powerLimiting_calcTotalPower( VD_TorqueToInv requested_torques , VD_WheelRpms rpms);
static inline bool app_powerLimiting_limitingRequired(double requested_power, double power_limit);

/************************* Global Functions *****************************/

powerLimiting_outputs app_powerLimiting(VD_TorqueToInv torques, VD_WheelRpms wheel_rpms)
{
    const powerLimiting_inputs in = app_powerLimiting_getInputs( torques, wheel_rpms);
    
    const powerLimiting_outputs out = {
        .total_requested_power = app_powerLimiting_calcTotalPower(in.torques, in.wheel_rpms),
        .power_limit = app_powerLimiting_calcMaxPower(in.isRegen_on),
        .per_wheel_torque_reduction = app_powerLimiting_calcTorqueReduction(in, out)
    };

    app_powerLimiting_broadCast(out);
    return out; 
}


/************************* Private Function Declaration *****************************/
static powerLimiting_inputs app_powerLimiting_getInputs(VD_TorqueToInv requested_torques, VD_WheelRpms wheel_rpms)
{
    const powerLimiting_inputs inputs = {
        .torques = requested_torques,
        .wheel_rpms = wheel_rpms,
        .isRegen_on = SWITCH_ON == app_canRx_CRIT_RegenSwitch_get(),
        .bms_power_limit =  inputs.isRegen_on ? app_canRx_BMS_ChargePowerLimit_get() : app_canRx_BMS_DischargePowerLimit_get(),
    };

    return inputs; 

}

static void app_powerLimiting_broadCast(powerLimiting_outputs out)
{
    app_canTx_VC_TotalRequestedPower_set(out.total_requested_power);
    app_canTx_VC_PowerLimit_set(out.power_limit);
    app_canTx_VC_PerWheelTorqueReduction_set(out.per_wheel_torque_reduction);

}

/************************* Helper Function Declaration *****************************/

static inline double app_powerLimiting_calcTorqueReduction(powerLimiting_inputs in, powerLimiting_outputs out)
{
    // TODO: regen power limiting and regen handling

    // the idea now is that all modulation of torque will mow be the responsibility of the torque allocator alone, there for power limiting, yaw, slip and dynamics est just provide it the info it 
    // needs to allocate torque
    double output = 0.0;
    if(out.total_requested_power > in.bms_power_limit)
    {
        const double total_wheel_rpm = TOTAL_WHEEL_RPM(in.wheel_rpms.wheel_rpm_fl, in.wheel_rpms.wheel_rpm_fr, in.wheel_rpms.wheel_rpm_rl, in.wheel_rpms.wheel_rpm_rr);
        output = POWER_TO_TORQUE(out.total_requested_power - out.power_limit, total_wheel_rpm);
    }

    return output;

}

static inline double app_powerLimiting_calcTotalPower( VD_TorqueToInv requested_torques , VD_WheelRpms rpms)
{
    return TORQUE_TO_POWER(requested_torques.torque_fl, rpms.wheel_rpm_fl) + 
           TORQUE_TO_POWER(requested_torques.torque_fl, rpms.wheel_rpm_fl) + 
           TORQUE_TO_POWER(requested_torques.torque_fl, rpms.wheel_rpm_fl) +
           TORQUE_TO_POWER(requested_torques.torque_fl, rpms.wheel_rpm_fl);
}

static inline double app_powerLimiting_calcMaxPower(bool isRegen_on)
{
    const double abs_p_max  = inputs.isRegen_on ? POWER_LIMIT_REGEN_kW : RULES_BASED_POWER_LIMIT_KW;
    return fmin(inputs.bms_power_limit, abs_p_max);
}