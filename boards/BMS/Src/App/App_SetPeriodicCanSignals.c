#include "App_SetPeriodicCanSignals.h"
#include "App_SharedSetPeriodicCanSignals.h"
#include "App_InRangeCheck.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(BmsCanTxInterface)

void App_SetPeriodicCanSignals_Imd(
    struct BmsCanTxInterface *can_tx,
    struct Imd *              imd)
{
    App_CanTx_SetPeriodicSignal_SECONDS_SINCE_POWER_ON(
        can_tx, App_Imd_GetSecondsSincePowerOn(imd));
    App_CanTx_SetPeriodicSignal_FREQUENCY(can_tx, App_Imd_GetPwmFrequency(imd));
    App_CanTx_SetPeriodicSignal_DUTY_CYCLE(
        can_tx, App_Imd_GetPwmDutyCycle(imd));

    const struct Imd_Condition condition = App_Imd_GetCondition(imd);
    App_CanTx_SetPeriodicSignal_CONDITION(can_tx, (uint8_t)condition.name);

    App_CanTx_SetPeriodicSignal_VALID_DUTY_CYCLE(
        can_tx, condition.pwm_encoding.valid_duty_cycle);

    switch (condition.name)
    {
        case IMD_NORMAL:
        {
            if (condition.pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_10_HZ(
                    can_tx,
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            if (condition.pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_20_HZ(
                    can_tx,
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_SST:
        {
            App_CanTx_SetPeriodicSignal_SPEED_START_STATUS_30_HZ(
                can_tx, condition.pwm_encoding.speed_start_status);
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

void App_SetPeriodicSignals_AccumulatorVoltagesInRangeChecks(
    struct BmsCanTxInterface *const can_tx,
    const struct Accumulator *const accumulator)
{
    App_Accumulator_ReadCellVoltages(accumulator);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetMinCellVoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE,
        App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetMaxCellVoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE,
        App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetAverageCellVoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_AVERAGE_CELL_VOLTAGE,
        App_CanTx_SetPeriodicSignal_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetPackVoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_PACK_VOLTAGE,
        App_CanTx_SetPeriodicSignal_PACK_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetSegment0VoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetSegment1VoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetSegment2VoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_SEGMENT_2_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_2_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_2_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_2_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_2_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetSegment3VoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_SEGMENT_3_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_3_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_3_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_3_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_3_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetSegment4VoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_SEGMENT_4_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_4_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_4_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_4_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_4_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetSegment5VoltageInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_SEGMENT_5_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_5_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_5_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_5_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_SEGMENT_5_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

void App_SetPeriodicSignals_AccumulatorTemperaturesInRangeChecks(
    struct BmsCanTxInterface *const can_tx,
    const struct Accumulator *const accumulator)
{
    App_Accumulator_ReadCellTemperatures(accumulator);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetMinCellTemperatureInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP,
        App_CanTx_SetPeriodicSignal_MIN_CELL_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, App_Accumulator_GetMaxCellTemperatureInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP,
        App_CanTx_SetPeriodicSignal_MAX_CELL_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx,
        App_Accumulator_GetAverageCellTemperatureInRangeCheck(accumulator),
        App_CanTx_SetPeriodicSignal_AVERAGE_CELL_TEMP,
        App_CanTx_SetPeriodicSignal_AVERAGE_CELL_TEMP_OUT_OF_RANGE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_AVERAGE_CELL_TEMP_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
