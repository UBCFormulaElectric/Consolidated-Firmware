#include "App_SetPeriodicCanSignals.h"

void App_SetPeriodicCanSignals_Imd(struct Imd *imd)
{
    App_CanTx_BMSImdStatus_SecondsSincePowerOn_Set(App_Imd_GetSecondsSincePowerOn(imd));
    App_CanTx_BMSImdPwmOutput_Frequency_Set(App_Imd_GetPwmFrequency(imd));
    App_CanTx_BMSImdPwmOutput_DutyCycle_Set(App_Imd_GetPwmDutyCycle(imd));

    const struct Imd_Condition condition = App_Imd_GetCondition(imd);
    // TODO: JSONCAN -> App_CanTx_SetPeriodicSignal_CONDITION(can_tx, (uint8_t)condition.name);

    App_CanTx_BMSImdStatus_ValidDutyCycle_Set(condition.pwm_encoding.valid_duty_cycle);

    switch (condition.name)
    {
        case IMD_NORMAL:
        {
            if (condition.pwm_encoding.valid_duty_cycle)
            {
                App_CanTx_BMSImdData_InsulationMeasurementDcp10Hz_Set(
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
                App_CanTx_BMSImdData_ActiveFrequency_Set(10);
            }
        }
        break;
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            if (condition.pwm_encoding.valid_duty_cycle)
            {
                App_CanTx_BMSImdData_InsulationMeasurementDcp20Hz_Set(
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
                App_CanTx_BMSImdData_ActiveFrequency_Set(20);
            }
        }
        break;
        case IMD_SST:
        {
            App_CanTx_BMSImdData_SpeedStartStatus30Hz_Set(condition.pwm_encoding.speed_start_status);
        }
        break;
        case IMD_SHORT_CIRCUIT:
        case IMD_DEVICE_ERROR:
        case IMD_GROUND_FAULT:
        case IMD_INVALID:
        {
            // Nothing to do for conditions that don't carry a payload
        }
        break;
    }
}
