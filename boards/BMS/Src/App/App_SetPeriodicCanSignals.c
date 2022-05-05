#include "App_SetPeriodicCanSignals.h"

void App_SetPeriodicCanSignals_Imd(struct BmsCanTxInterface *can_tx, struct Imd *imd)
{
    App_CanTx_SetPeriodicSignal_SECONDS_SINCE_POWER_ON(can_tx, App_Imd_GetSecondsSincePowerOn(imd));
    App_CanTx_SetPeriodicSignal_FREQUENCY(can_tx, App_Imd_GetPwmFrequency(imd));
    App_CanTx_SetPeriodicSignal_DUTY_CYCLE(can_tx, App_Imd_GetPwmDutyCycle(imd));

    const struct Imd_Condition condition = App_Imd_GetCondition(imd);
    App_CanTx_SetPeriodicSignal_CONDITION(can_tx, (uint8_t)condition.name);

    App_CanTx_SetPeriodicSignal_VALID_DUTY_CYCLE(can_tx, condition.pwm_encoding.valid_duty_cycle);

    switch (condition.name)
    {
        case IMD_NORMAL:
        {
            if (condition.pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_10_HZ(
                    can_tx, condition.pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            if (condition.pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_20_HZ(
                    can_tx, condition.pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_SST:
        {
            App_CanTx_SetPeriodicSignal_SPEED_START_STATUS_30_HZ(can_tx, condition.pwm_encoding.speed_start_status);
        }
        break;
        case IMD_SHORT_CIRCUIT:
        case IMD_DEVICE_ERROR:
        case IMD_EARTH_FAULT:
        case IMD_INVALID:
        {
            // Nothing to do for conditions that don't carry a payload
        }
        break;
    }
}
