#include "App_SetPeriodicCanSignals.h"

void App_SetPeriodicCanSignals_Imd(struct Imd *imd)
{
    App_CanTx_BMS_ImdSecondsSincePowerOn_Set(App_Imd_GetSecondsSincePowerOn(imd));
    App_CanTx_BMS_ImdFrequency_Set(App_Imd_GetPwmFrequency(imd));
    App_CanTx_BMS_ImdDutyCycle_Set(App_Imd_GetPwmDutyCycle(imd));

    const struct Imd_Condition condition = App_Imd_GetCondition(imd);
    App_CanTx_BMS_ImdCondition_Set((uint8_t)condition.name);
    App_CanTx_BMS_ImdValidDutyCycle_Set(condition.pwm_encoding.valid_duty_cycle);

    switch (condition.name)
    {
        case IMD_NORMAL:
        {
            if (condition.pwm_encoding.valid_duty_cycle)
            {
                App_CanTx_BMS_ImdInsulationMeasurementDcp10Hz_Set(condition.pwm_encoding.insulation_measurement_dcp_kohms);
                App_CanTx_BMS_ImdActiveFrequency_Set(IMD_10Hz);
            }
        }
        break;
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            if (condition.pwm_encoding.valid_duty_cycle)
            {
                App_CanTx_BMS_ImdInsulationMeasurementDcp20Hz_Set(condition.pwm_encoding.insulation_measurement_dcp_kohms);
                App_CanTx_BMS_ImdActiveFrequency_Set(IMD_20Hz);
            }
        }
        break;
        case IMD_SST:
        {
            App_CanTx_BMS_ImdSpeedStartStatus30Hz_Set(condition.pwm_encoding.speed_start_status);
        }
        break;
        default:
        {
            // Nothing to do for conditions that don't carry a payload
        }
        break;
    }
}
